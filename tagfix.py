#!/usr/bin/env python
import os
from shutil import move
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument('path', type=str)
parser.add_argument('find', type=str)
parser.add_argument('replace', type=str)
args = parser.parse_args()

sourcedir = os.path.abspath(args.path)

for root, dirs, files in os.walk(sourcedir):
    for f in files:
        if f.endswith('csv'):
            src_path = root + '/' + f
            dst_path = root + '/' + f + '.tmp'
            src = open(src_path, 'rt')
            dst = open(dst_path, 'wt')
            for line in src:
                line = line.replace(args.find, args.replace)
                if len(line) > 0:
                    dst.write(line)
            src.close()
            dst.close()
            move(dst_path, src_path)