#-------------------------------------------------------------------------------
# Copyright (c) 2018-2024, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

import os
import io
import re
import sys
import argparse
import logging
from jinja2 import Environment, BaseLoader, select_autoescape, TemplateNotFound

try:
    import yaml
except ImportError as e:
    logging.error (str(e) + " To install it, type:")
    logging.error ("pip install PyYAML")
    exit(1)

donotedit_warning = \
                    '  WARNING: This is an auto-generated file. Do not edit!  '

# 128 distinct values, all with the top bits set to 0x5555 and a Hamming distance of 6
# With NSIds all having 0xaaaa for the top 16 bits, we have a Hamming distance of 16
# between NSIDs and PIDs and it should be difficult to glitch an NSID into a PID
VALID_PIDS = [
0x55550000, 0x5555003f, 0x555501c7, 0x555501f8, 0x5555064b, 0x55550674, 0x5555078c, 0x555507b3,
0x55550a95, 0x55550aaa, 0x55550b52, 0x55550b6d, 0x55550cde, 0x55550ce1, 0x55550d19, 0x55550d26,
0x5555304d, 0x55553072, 0x5555318a, 0x555531b5, 0x55553606, 0x55553639, 0x555537c1, 0x555537fe,
0x55553ad8, 0x55553ae7, 0x55553b1f, 0x55553b20, 0x55553c93, 0x55553cac, 0x55553d54, 0x55553d6b,
0x55555096, 0x555550a9, 0x55555151, 0x5555516e, 0x555556dd, 0x555556e2, 0x5555571a, 0x55555725,
0x55555a03, 0x55555a3c, 0x55555bc4, 0x55555bfb, 0x55555c48, 0x55555c77, 0x55555d8f, 0x55555db0,
0x555560db, 0x555560e4, 0x5555611c, 0x55556123, 0x55556690, 0x555566af, 0x55556757, 0x55556768,
0x55556a4e, 0x55556a71, 0x55556b89, 0x55556bb6, 0x55556c05, 0x55556c3a, 0x55556dc2, 0x55556dfd,
0x55559257, 0x55559268, 0x55559390, 0x555593af, 0x5555941c, 0x55559423, 0x555595db, 0x555595e4,
0x555598c2, 0x555598fd, 0x55559905, 0x5555993a, 0x55559e89, 0x55559eb6, 0x55559f4e, 0x55559f71,
0x5555a21a, 0x5555a225, 0x5555a3dd, 0x5555a3e2, 0x5555a451, 0x5555a46e, 0x5555a596, 0x5555a5a9,
0x5555a88f, 0x5555a8b0, 0x5555a948, 0x5555a977, 0x5555aec4, 0x5555aefb, 0x5555af03, 0x5555af3c,
0x5555c2c1, 0x5555c2fe, 0x5555c306, 0x5555c339, 0x5555c48a, 0x5555c4b5, 0x5555c54d, 0x5555c572,
0x5555c854, 0x5555c86b, 0x5555c993, 0x5555c9ac, 0x5555ce1f, 0x5555ce20, 0x5555cfd8, 0x5555cfe7,
0x5555f28c, 0x5555f2b3, 0x5555f34b, 0x5555f374, 0x5555f4c7, 0x5555f4f8, 0x5555f500, 0x5555f53f,
0x5555f819, 0x5555f826, 0x5555f9de, 0x5555f9e1, 0x5555fe52, 0x5555fe6d, 0x5555ff95, 0x5555ffaa,
]

TFM_ROOT_DIR = os.path.join(sys.path[0], '..')
OUT_DIR = None # The root directory that files are generated to

# PID[0, TFM_PID_BASE - 1] are reserved for TF-M SPM and test usages
TFM_PID_BASE = VALID_PIDS[16]

# variable for checking for duplicated sid
sid_list = []

# Summary of manifest attributes defined by FFM for use in the Secure Partition manifest file.
ffm_manifest_attributes = ['psa_framework_version', 'name', 'type', 'priority', 'model', 'entry_point', \
'stack_size', 'description', 'entry_init', 'heap_size', 'mmio_regions', 'services', 'irqs', 'dependencies',\
'client_id_base', 'client_id_limit']

class TemplateLoader(BaseLoader):
    """
    Template loader class.

    An instance of this class is passed to the template engine. It is
    responsible for reading the template file
    """
    def __init__(self):
        pass

    def get_source(self, environment, template):
        """
        This function reads the template files.
        For detailed documentation see:
        http://jinja.pocoo.org/docs/2.10/api/#jinja2.BaseLoader.get_source

        Please note that this function always return 'false' as 'uptodate'
        value, so the output file will always be generated.
        """
        if not os.path.isfile(template):
            raise TemplateNotFound(template)
        with open(template) as f:
            source = f.read()
        return source, template, False

def parse_configurations(file_paths):
    """
    Parses the given config files and return a dict whose key-values are build
    configurations and their values.

    Valid configurations should be in the format of:
    "#define VAR [...]" in a single line.
    The value of the config is optional.
    """
    configurations = {}

    lines = []
    for file in file_paths:
        with open(file, 'r') as config_file:
            lines += config_file.readlines()

    for line in lines:
        if not line.startswith('#define'):
            continue

        line = line.rstrip('\r\n')
        line_items = line.split(maxsplit=2)
        if len(line_items) == 3:
            configurations[line_items[1]] = line_items[2]
        elif len(line_items) == 2:
            configurations[line_items[1]] = ''

    logging.debug(configurations)

    return configurations

def manifest_validation(manifest, pid):
    """
    This function validates FF-M compliance for partition manifest, and sets
    default values for optional attributes.
    The validation is skipped for TF-M specific Partitions (PID < TFM_PID_BASE).
    """

    service_list = manifest.get('services', [])
    irq_list     = manifest.get('irqs', [])

    # "psa_framework_version" validation
    if manifest['psa_framework_version'] not in [1.0, 1.1]:
        raise Exception('Invalid psa_framework_version of {}'.format(manifest['name']))

    # "type" validation
    if manifest['type'] not in ['PSA-ROT', 'APPLICATION-ROT']:
        raise Exception('Invalid type of {}'.format(manifest['name']))

    # "priority" validation
    if manifest['priority'] not in ['HIGH', 'NORMAL', 'LOW']:
        raise Exception('Invalid priority of {}'.format(manifest['name']))

    # "pid" validation
    if (pid != None) and (pid not in VALID_PIDS):
        raise Exception('Invalid pid of {}'.format(manifest['name']))

    if 'ns_agent' not in manifest:
        manifest['ns_agent'] = False

    # Every PSA Partition must have at least either a secure service or an IRQ
    if (pid == None or pid >= TFM_PID_BASE) \
       and len(service_list) == 0 and len(irq_list) == 0:
        raise Exception('{} must declare at least either a secure service or an IRQ!'
                        .format(manifest['name']))

    if manifest['psa_framework_version'] == 1.0:
        # For 1.0 Partition, the model is IPC
        manifest['model'] = 'IPC'

    # "model" validation:
    model = manifest.get('model', None)
    if model == None:
        raise Exception('{} is missing the "model" attribute'.format(manifest['name']))

    # Assign a unified 'entry' for templates
    if model == 'IPC':
        # entry_point is mandatory for IPC Partitions
        if 'entry_point' not in manifest.keys():
            raise Exception('{} is missing the "entry_point" attribute'.format(manifest['name']))
        manifest['entry'] = manifest['entry_point']
    elif model == 'SFN':
        if 'entry_init' in manifest.keys():
            manifest['entry'] = manifest['entry_init']
        else:
            manifest['entry'] = 0
    else:
        raise Exception('Invalid "model" of {}'.format(manifest['name']))

    # Service FF-M manifest validation
    for service in service_list:
        if manifest['psa_framework_version'] == 1.0:
            service['connection_based'] = True
        elif 'connection_based' not in service:
            raise Exception("'connection_based' is mandatory in FF-M 1.1 service!")

        if 'version' not in service.keys():
            service['version'] = 1
        if 'version_policy' not in service.keys():
            service['version_policy'] = 'STRICT'

        # SID duplication check
        if service['sid'] in sid_list:
            raise Exception('Service ID: {} has duplications!'.format(service['sid']))
        else:
            sid_list.append(service['sid'])

    return manifest

def check_circular_dependency(partitions, service_partition_map):
    """
    This function detects if there is any circular partition dependency chain.
    If a circular dependency is detected, the script exits with error.

    Inputs:
        - partitions:            dict of partition manifests
        - service_partition_map: map between services and their owner Partitions
    """

    dependency_table = {}
    for partition in partitions:
        manifest = partition['manifest']
        dependencies = manifest['dependencies'].copy() \
                       if 'dependencies' in manifest else []
        dependencies += manifest['weak_dependencies'].copy() \
                        if 'weak_dependencies' in manifest else []
        dependency_table[manifest['name']] = {
            'dependencies': [service_partition_map[dependency]
                             for dependency in dependencies
                             if dependency in service_partition_map],
            'validated': False
        }

    for partition in dependency_table.keys():
        validate_dependency_chain(partition, dependency_table, [])

def validate_dependency_chain(partition,
                              dependency_table,
                              dependency_chain):
    """
    Recursively validate if the given partition and its dependencies
    have a circular dependency with the given dependency_chain.
    Exit with error code once any circular is detected.

    Inputs:
        - partition:        next partition to be checked
        - dependency_table: dict of partitions and their dependencies
        - dependency_chain: list of dependencies in current chain
    """

    dependency_chain.append(partition)
    if partition in dependency_chain[:-1]:
        logging.error(
            'Circular dependency exists in chain: {}'.format(
                ', '.join(dependency_chain)))
        exit(1)
    for dependency in dependency_table[partition]['dependencies']:
        if dependency_table[dependency]['validated']:
            continue
        validate_dependency_chain(dependency, dependency_table, dependency_chain)
    dependency_table[partition]['validated'] = True

def manifest_attribute_check(manifest, manifest_item):
    """
    Check whether Non-FF-M compliant attributes are explicitly registered in manifest lists.

    Inputs:
        - manifest:        next manifest to be checked
        - manifest_item:   the manifest items in manifest lists
    """
    allowed_attributes = ffm_manifest_attributes + manifest_item.get('non_ffm_attributes', [])
    for keyword in manifest.keys():
        if keyword not in allowed_attributes:
            logging.error('The Non-FFM attribute {} is used by {} without registration.'.format(keyword, manifest['name']))
            exit(1)

def process_partition_manifests(manifest_lists, configs):
    """
    Parse the input manifest lists, check if manifest settings are valid,
    generate the data base for generated files
    and generate manifest header files.

    Parameters
    ----------
    manifest_lists:
        A list of Secure Partition manifest lists

    Returns
    -------
    The manifest data base.
    """

    context = {}

    partition_list = []
    all_manifests = []
    pid_list = []
    no_pid_manifest_idx = []
    service_partition_map = {}
    partition_statistics = {
        'connection_based_srv_num': 0,
        'ipc_partitions': [],
        'mmio_region_num': 0,
        'flih_num': 0,
        'slih_num': 0
    }
    config_impl = {
        'CONFIG_TFM_SPM_BACKEND_SFN'              : '0',
        'CONFIG_TFM_SPM_BACKEND_IPC'              : '0',
        'CONFIG_TFM_CONNECTION_BASED_SERVICE_API' : '0',
        'CONFIG_TFM_MMIO_REGION_ENABLE'           : '0',
        'CONFIG_TFM_FLIH_API'                     : '0',
        'CONFIG_TFM_SLIH_API'                     : '0'
    }
    priority_map = {
        'LOWEST'              : '00',
        'LOW'                 : '01',
        'NORMAL'              : '02',
        'HIGH'                : '03',
        'HIGHEST'             : '04'
    }

    isolation_level = int(configs['TFM_ISOLATION_LEVEL'], base = 10)
    backend = configs['CONFIG_TFM_SPM_BACKEND']

    # Get all the manifests information as a dictionary
    for i, item in enumerate(manifest_lists):
        if not os.path.isfile(item):
            logging.error('Manifest list item [{}] must be a file'.format(i))
            exit(1)

        # The manifest list file generated by configure_file()
        with open(item) as manifest_list_yaml_file:
            manifest_dic = yaml.safe_load(manifest_list_yaml_file)['manifest_list']
            for dict in manifest_dic:
                # Replace environment variables in the manifest path.
                expanded_path = os.path.expandvars(dict['manifest']).replace('\\', '/')

                # If the manifest exists relative to the manifest list, then use
                # that. Else, either interpret it as an absolute path or one
                # relative to the current working directory
                path_relative_to_manifest_list = os.path.join(os.path.dirname(item), # path of manifest list
                                                              expanded_path)
                if os.path.isfile(path_relative_to_manifest_list):
                    manifest_path = path_relative_to_manifest_list
                else:
                    manifest_path = expanded_path
                dict['manifest'] = manifest_path
                all_manifests.append(dict)

    logging.info("------------ Display partition configuration - start ------------")

    # Parse the manifests
    for i, manifest_item in enumerate(all_manifests):
        valid_enabled_conditions  = ['1', 'on',  'true',  'enabled']
        valid_disabled_conditions = ['0', 'off', 'false', 'disabled', '']
        is_enabled = ''

        if 'conditional' in manifest_item.keys():
            if manifest_item['conditional'] not in configs.keys():
                logging.error('Configuration "{}" is not defined!'.format(manifest_item['conditional']))
                exit(1)
            is_enabled = configs[manifest_item['conditional']].lower()
        else:
            # Partitions without 'conditional' is always on
            is_enabled = '1'

        if is_enabled in valid_disabled_conditions:
            logging.info("   {:40s}  OFF".format(manifest_item['description']))
            continue
        elif is_enabled in valid_enabled_conditions:
            logging.info("   {:40s}  ON".format(manifest_item['description']))
        else:
            raise Exception('Invalid "conditional" attribute: "{}" for {}. '
                            'Please set to one of {} or {}, case-insensitive.'\
                            .format(manifest_item['conditional'],
                                    manifest_item['description'],
                                    valid_enabled_conditions, valid_disabled_conditions))

        # Check if partition ID is manually set
        if 'pid' not in manifest_item.keys():
            no_pid_manifest_idx.append(i)
            pid = None
        else:
            pid = manifest_item['pid']

            # Check if partition ID is duplicated
            if pid in pid_list:
                raise Exception('PID No. {} has already been used!'.format(pid))
            else:
                pid_list.append(pid)

        manifest_path = manifest_item['manifest']
        with open(manifest_path) as manifest_file:
            manifest = yaml.safe_load(manifest_file)
            # Check manifest attribute validity
            manifest_attribute_check(manifest, manifest_item)

            if manifest.get('model', None) == 'dual':
                # If a Partition supports both models, it can set the "model" to "backend".
                # The actual model used follows the backend being used.
                manifest['model'] = backend
            manifest = manifest_validation(manifest, pid)

            # Priority mapping
            numbered_priority = priority_map[manifest['priority']]

        if (pid == None or pid >= TFM_PID_BASE) and not manifest['ns_agent']:
            # Count the number of IPC/SFN partitions (excluding TF-M internal
            # and agent partitions)
            if manifest['model'] == 'IPC':
                partition_statistics['ipc_partitions'].append(manifest['name'])

        # Set initial value to -1 to make (srv_idx + 1) reflect the correct
        # number (0) when there are no services.
        srv_idx = -1
        for srv_idx, service in enumerate(manifest.get('services', [])):
            service_partition_map[service['name']] = manifest['name']
            if manifest['model'] == 'IPC':
                # Assign signal value, the first 4 bits are reserved by FF-M
                service['signal_value'] = (1 << (srv_idx + 4))
            else:
                # Signals of SFN Partitions are SPM internal only, does not
                # need to reserve 4 bits.
                service['signal_value'] = (1 << srv_idx)
            if service['connection_based']:
                partition_statistics['connection_based_srv_num'] += 1
        logging.debug('{} has {} services'.format(manifest['name'], srv_idx +1))

        # Calculate the number of mmio region
        mmio_region_list = manifest.get('mmio_regions', [])
        partition_statistics['mmio_region_num'] += len(mmio_region_list)

        # Set initial value to -1 to make (irq + 1) reflect the correct
        # number (0) when there are no irqs.
        irq_idx = -1
        for irq_idx, irq in enumerate(manifest.get('irqs', [])):
            # Assign signal value, from the most significant bit
            irq['signal_value'] = (1 << (31 - irq_idx))
            if irq.get('handling', None) == 'FLIH':
                partition_statistics['flih_num'] += 1
            else:
                partition_statistics['slih_num'] += 1
        logging.debug('{} has {} IRQS'.format(manifest['name'], irq_idx +1))

        if ((srv_idx + 1) + (irq_idx + 1)) > 28:
            raise Exception('Total number of Services and IRQs of {} exceeds the limit (28)'
                            .format(manifest['name']))

        manifest_out_basename = os.path.splitext(os.path.basename(manifest_path))[0]

        if 'output_path' in manifest_item:
            output_path = os.path.expandvars(manifest_item['output_path'])
        else:
            output_path = ''

        manifest_head_file = os.path.join(OUT_DIR, output_path, 'psa_manifest',
                                          '{}.h'.format(manifest_out_basename))\
                                              .replace('\\', '/')
        intermedia_file    = os.path.join(OUT_DIR, output_path, 'auto_generated',
                                          'intermedia_{}.c'.format(manifest_out_basename))\
                                              .replace('\\', '/')
        load_info_file     = os.path.join(OUT_DIR, output_path, 'auto_generated',
                                          'load_info_{}.c'.format(manifest_out_basename))\
                                              .replace('\\', '/')
        output_dir         = os.path.join(OUT_DIR, output_path).replace('\\', '/')

        partition_list.append({'manifest': manifest, 'attr': manifest_item,
                               'manifest_out_basename': manifest_out_basename,
                               'header_file': manifest_head_file,
                               'intermedia_file': intermedia_file,
                               'loadinfo_file': load_info_file,
                               'output_dir': output_dir,
                               'numbered_priority': numbered_priority})

    logging.info("------------ Display partition configuration - end ------------")

    check_circular_dependency(partition_list, service_partition_map)

    # Automatically assign PIDs for partitions without 'pid' attribute
    for idx in no_pid_manifest_idx:
        for pid in VALID_PIDS:
            if (pid >= TFM_PID_BASE) and (pid not in pid_list):
                all_manifests[idx]['pid'] = pid
                pid_list.append(pid)
                break
        else:
            logging.error('Ran out of PIDs')
            exit(1)

    # Set up configurations
    if backend == 'SFN':
        if len(partition_statistics['ipc_partitions']) > 0:
            logging.error('SFN backend does not support IPC Partitions:')
            logging.error(partition_statistics['ipc_partitions'])
            exit(1)

        if isolation_level > 1:
            logging.error('SFN backend does not support high isolation levels.')
            exit(1)

        config_impl['CONFIG_TFM_SPM_BACKEND_SFN'] = '1'
    elif backend == 'IPC':
        config_impl['CONFIG_TFM_SPM_BACKEND_IPC'] = '1'

    if partition_statistics['connection_based_srv_num'] > 0:
        config_impl['CONFIG_TFM_CONNECTION_BASED_SERVICE_API'] = 1

    if partition_statistics['mmio_region_num'] > 0:
        config_impl['CONFIG_TFM_MMIO_REGION_ENABLE'] = 1

    if partition_statistics['flih_num'] > 0:
        config_impl['CONFIG_TFM_FLIH_API'] = 1
    if partition_statistics['slih_num'] > 0:
        config_impl['CONFIG_TFM_SLIH_API'] = 1

    context['partitions'] = partition_list
    context['config_impl'] = config_impl
    context['stateless_services'] = process_stateless_services(partition_list)

    return context

def gen_per_partition_files(context):
    """
    Generate per-partition files

    Parameters
    ----------
    context:
        context contains partition infos
    """

    partition_context = {}
    partition_context['utilities'] = context['utilities']
    partition_context['config_impl'] = context['config_impl']

    manifesttemplate = ENV.get_template(os.path.join(sys.path[0], 'templates/manifestfilename.template'))
    memorytemplate = ENV.get_template(os.path.join(sys.path[0], 'templates/partition_intermedia.template'))
    infotemplate = ENV.get_template(os.path.join(sys.path[0], 'templates/partition_load_info.template'))

    logging.info ("Start to generate partition files:")

    for one_partition in context['partitions']:
        partition_context['manifest'] = one_partition['manifest']
        partition_context['attr'] = one_partition['attr']
        partition_context['manifest_out_basename'] = one_partition['manifest_out_basename']
        partition_context['numbered_priority'] = one_partition['numbered_priority']

        logging.info ('Generating {} in {}'.format(one_partition['attr']['description'],
                                            one_partition['output_dir']))
        outfile_path = os.path.dirname(one_partition['header_file'])
        if not os.path.exists(outfile_path):
            os.makedirs(outfile_path)

        headerfile = io.open(one_partition['header_file'], 'w', newline=None)
        headerfile.write(manifesttemplate.render(partition_context))
        headerfile.close()

        intermediafile_path = os.path.dirname(one_partition['intermedia_file'])
        if not os.path.exists(intermediafile_path):
            os.makedirs(intermediafile_path)
        intermediafile = io.open(one_partition['intermedia_file'], 'w', newline=None)
        intermediafile.write(memorytemplate.render(partition_context))
        intermediafile.close()

        infofile_path = os.path.dirname(one_partition['loadinfo_file'])
        if not os.path.exists(infofile_path):
            os.makedirs(infofile_path)
        infooutfile = io.open(one_partition['loadinfo_file'], 'w', newline=None)
        infooutfile.write(infotemplate.render(partition_context))
        infooutfile.close()

    logging.info ("Per-partition files done:")

def gen_summary_files(context, gen_file_lists):
    """
    Generate files according to the gen_file_list

    Parameters
    ----------
    gen_file_lists:
        The lists of files to generate
    """
    file_list = []

    for f in gen_file_lists:
        with open(f) as file_list_yaml_file:
            file_list_yaml = yaml.safe_load(file_list_yaml_file)
            file_list.extend(file_list_yaml['file_list'])

    for file in file_list:
        # Replace environment variables in the output filepath
        manifest_out_file = os.path.expandvars(file['output'])
        # Replace environment variables in the template filepath
        templatefile_name = os.path.expandvars(file['template'])

        manifest_out_file = os.path.join(OUT_DIR, manifest_out_file)

        outfile_path = os.path.dirname(manifest_out_file)
        if not os.path.exists(outfile_path):
            os.makedirs(outfile_path)

        template = ENV.get_template(templatefile_name)

        outfile = io.open(manifest_out_file, 'w', newline=None)
        outfile.write(template.render(context))
        outfile.close()

def process_stateless_services(partitions):
    """
    This function collects all stateless services together, and allocates
    stateless handles for them.
    Valid stateless handle in service will be converted to an index. If the
    stateless handle is set as "auto", or not set, framework will allocate a
    valid index for the service.
    Framework puts each service into a reordered stateless service list at
    position of "index". Other unused positions are left None.

    Keep the variable names start with upper case 'STATIC_HANDLE_' the same
    as the preprocessors in C sources. This could easier the upcomping
    modification when developer searches these definitions for modification.
    """

    collected_stateless_services = []
    STATIC_HANDLE_NUM_LIMIT = 32

    # Collect all stateless services first.
    for partition in partitions:
        # Skip the FF-M 1.0 partitions
        if partition['manifest']['psa_framework_version'] < 1.1:
            continue

        service_list = partition['manifest'].get('services', [])

        for service in service_list:
            if service['connection_based'] is False:
                collected_stateless_services.append(service)

    if len(collected_stateless_services) == 0:
        return []

    if len(collected_stateless_services) > STATIC_HANDLE_NUM_LIMIT:
        raise Exception('Stateless service numbers range exceed {number}.'.format(number=STATIC_HANDLE_NUM_LIMIT))

    """
    Allocate an empty stateless service list to store services.
    Use "handle - 1" as the index for service, since handle value starts from
    1 and list index starts from 0.
    """
    reordered_stateless_services = [None] * STATIC_HANDLE_NUM_LIMIT
    auto_alloc_services = []

    for service in collected_stateless_services:
        # If not set, it is "auto" by default
        if 'stateless_handle' not in service:
            auto_alloc_services.append(service)
            continue

        service_handle = service['stateless_handle']

        # Fill in service list with specified stateless handle, otherwise skip
        if isinstance(service_handle, int):
            if service_handle < 1 or service_handle > STATIC_HANDLE_NUM_LIMIT:
                raise Exception('Invalid stateless_handle setting: {handle}.'.format(handle=service['stateless_handle']))
            # Convert handle index to reordered service list index
            service_handle = service_handle - 1

            if reordered_stateless_services[service_handle] is not None:
                raise Exception('Duplicated stateless_handle setting: {handle}.'.format(handle=service['stateless_handle']))
            reordered_stateless_services[service_handle] = service
        elif service_handle == 'auto':
            auto_alloc_services.append(service)
        else:
            raise Exception('Invalid stateless_handle setting: {handle}.'.format(handle=service['stateless_handle']))

    STATIC_HANDLE_IDX_BIT_WIDTH = 5
    STATIC_HANDLE_IDX_MASK = (1 << STATIC_HANDLE_IDX_BIT_WIDTH) - 1
    STATIC_HANDLE_INDICATOR_OFFSET = 30
    STATIC_HANDLE_VER_OFFSET = 8
    STATIC_HANDLE_VER_BIT_WIDTH = 8
    STATIC_HANDLE_VER_MASK = (1 << STATIC_HANDLE_VER_BIT_WIDTH) - 1

    # Auto-allocate stateless handle and encode the stateless handle
    for i in range(0, STATIC_HANDLE_NUM_LIMIT):
        service = reordered_stateless_services[i]

        if service == None and len(auto_alloc_services) > 0:
            service = auto_alloc_services.pop(0)

        """
        Encode stateless flag and version into stateless handle
        """
        stateless_handle_value = 0
        if service != None:
            stateless_index = (i & STATIC_HANDLE_IDX_MASK)
            stateless_handle_value |= stateless_index
            stateless_handle_value |= (1 << STATIC_HANDLE_INDICATOR_OFFSET)
            stateless_version = (service['version'] & STATIC_HANDLE_VER_MASK) << STATIC_HANDLE_VER_OFFSET
            stateless_handle_value |= stateless_version
            service['stateless_handle_value'] = '0x{0:08x}'.format(stateless_handle_value)
            service['stateless_handle_index'] = stateless_index

        reordered_stateless_services[i] = service

    return reordered_stateless_services

def parse_args():
    parser = argparse.ArgumentParser(description='Parse secure partition manifest list and generate files listed by the file list',
                                     epilog='Note that environment variables in template files will be replaced with their values')

    parser.add_argument('-o', '--outdir'
                        , dest='outdir'
                        , required=True
                        , metavar='out_dir'
                        , help='The root directory for generated files')

    parser.add_argument('-m', '--manifest-lists'
                        , nargs='+'
                        , dest='manifest_lists'
                        , required=True
                        , metavar='manifest list'
                        , help='A list of Secure Partition manifest lists and their original paths.\n\
                                The manifest lists might be processed by CMake and\n\
                                the path might be different to the original one\n\
                                The format must be [list A, orignal path A, list B, orignal path B, ...]')

    parser.add_argument('-f', '--file-list'
                        , nargs='+'
                        , dest='gen_file_args'
                        , required=True
                        , metavar='file-list'
                        , help='These files describe the file list to generate')

    parser.add_argument('-c', '--config-files'
                        , nargs='+'
                        , dest='config_files'
                        , required=True
                        , metavar='config-files'
                        , help='A header file contains build configurations')

    parser.add_argument('-q', '--quiet'
                        , dest='quiet'
                        , required=False
                        , default=False
                        , action='store_true'
                        , help='Reduce log messages')

    args = parser.parse_args()

    return args

ENV = Environment(
        loader = TemplateLoader(),
        autoescape = select_autoescape(['html', 'xml']),
        lstrip_blocks = True,
        trim_blocks = True,
        keep_trailing_newline = True
    )

def main():
    """
    The entry point of the script.

    Generates the output files based on the templates and the manifests.
    """

    global OUT_DIR

    args = parse_args()

    logging.basicConfig(format='%(message)s'
                        , level=logging.WARNING if args.quiet else logging.INFO)

    OUT_DIR = os.path.abspath(args.outdir)

    manifest_lists = [os.path.abspath(x) for x in args.manifest_lists]
    gen_file_lists = [os.path.abspath(x) for x in args.gen_file_args]

    """
    Relative path to TF-M root folder is supported in the manifests
    and default value of manifest list and generated file list are relative to TF-M root folder as well,
    so first change directory to TF-M root folder.
    By doing this, the script can be executed anywhere
    The script is located in <TF-M root folder>/tools, so sys.path[0]<location of the script>/.. is TF-M root folder.
    """
    os.chdir(os.path.join(sys.path[0], '..'))

    context = process_partition_manifests(manifest_lists,
                                          parse_configurations(args.config_files))

    utilities = {}
    utilities['donotedit_warning'] = donotedit_warning

    context['utilities'] = utilities

    gen_per_partition_files(context)
    gen_summary_files(context, gen_file_lists)

if __name__ == '__main__':
    main()
