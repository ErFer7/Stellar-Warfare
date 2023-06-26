# -*- coding: utf-8 -*-

'''
Escaneia por leaks do código fonte.
'''

from os import walk

dir_check_list = ['../include', '../src', '../os/include', '../os/src']
files_to_check = []
found_files = []

log_file_lines = []

with open('minimalraw.log', 'r', encoding='utf-8') as log:
    log_file_lines = log.readlines()

for directory in dir_check_list:
    for root, dirs, files in walk(directory):
        for file in files:
            if file.endswith('.cc') or file.endswith('.h'):
                files_to_check.append(file)

for i, line in enumerate(log_file_lines):
    for file in files_to_check:
        if file in line and file not in found_files:
            found_files.append(file)

for file in found_files:
    print(file)
