#!/usr/bin/env python
# Python Cropper Tool
import os
import csv
import cv2
import math
import argparse
import numpy as np
import multiprocessing as mp
from shutil import rmtree

parser = argparse.ArgumentParser()
parser.add_argument('src', help='carpeta donde se encuentran las imagenes de origen', type=str)
parser.add_argument('--max-cpu', '-m', action='store', dest='maxcpu', help='numero maximo de procesos permitidos', type=int, default=mp.cpu_count())
parser.add_argument('--quality', '-q', action='store', dest='quality', help='calidad de los recortes', type=str, nargs='+')
parser.add_argument('--tags', '-t', action='store', dest='tags', help='etiquetas de los recortes', type=str, nargs='+')
parser.add_argument('--minimum', type=int, help='eliminar recortes menores a esta cantidad', default=0)
parser.add_argument('--maximum', type=int, help='rasurar carpetas donde el numero de archivos sea mayor que N', default=0)
args = parser.parse_args()

got_tags = False
if args.tags is not None:
    got_tags = len(args.tags) > 0
    args.tags = [x.upper() for x in args.tags]
got_qs = False
if args.quality is not None:
    got_qs = len(args.quality) > 0
    args.quality = [x.upper() for x in args.quality]


def csvProcesser(filedata: tuple):
    filepath, output_dir = filedata

    with open(filepath, 'rt') as csvfile:
        csvdata = csv.reader(csvfile, delimiter=',')
        crop_count = 1
        img_path = filepath[:-4].replace('_csv', '').replace('*', '')

        slashpos = filepath.rfind('/')
        stol_dot = filepath[:-4].rfind('.')
        img_name = filepath[slashpos+1:stol_dot]
        img_extension = filepath[stol_dot:-4]

        img = cv2.imread(img_path)

        # work out necessary rotations...
        rotations = 0
        for row in csvdata:
            if row[0][0] == '#':
                rotations = int(int(row[0][1:])/90)
                break
        # ... then rotate
        for _ in range(rotations):
            img = cv2.flip(cv2.transpose(img), 1)

        csvfile.seek(0)
        # process crops
        for row in csvdata:
            if row[0][0] == '#' or row[0][0] == '$' or row[0][0] == '%':
                continue
            # skip crop if it doesn't match specified tags and/or quality (if any)
            if got_tags:
                if row[4].upper() not in args.tags:
                    continue
            if got_qs:
                if row[5].upper() not in args.quality:
                    continue
            x1 = int(row[0])
            y1 = int(row[1])
            x2 = int(row[2])
            y2 = int(row[3])
            # crop image, save, close csv
            w = x2 - x1
            h = y2 - y1

            split_outdir = output_dir[:-1] + '_' + row[4].upper() + '/'

            if w > h:
                padding = (w - h)/2
                upper_padding = math.ceil(padding)
                lower_padding = math.floor(padding)
                target_crop = cv2.copyMakeBorder(img, upper_padding, lower_padding, 0, 0, cv2.BORDER_CONSTANT, None, [0, 0, 0])
            elif h > w:
                padding = (h - w)/2
                left_padding = math.ceil(padding)
                right_padding = math.floor(padding)
                target_crop = cv2.copyMakeBorder(img, 0, 0, left_padding, right_padding, cv2.BORDER_CONSTANT, None, [0, 0, 0])
            else:
                target_crop = img[y1:y1+h, x1:x1+w]
            
            os.makedirs(split_outdir, exist_ok=True)
            dest_file = split_outdir + img_name + '_' + str(crop_count).zfill(3)+img_extension
            crop_count += 1
            print('Guardando {}'.format(dest_file), flush=True)
            cv2.imwrite(dest_file, target_crop)
            del target_crop
        csvfile.close()


def main():
    if args.maxcpu <= 0:
        print('Numero de CPUs no valido.')
        quit()
    # loop over files in source dir, put CSV files in list
    csv_list = []

    source_dir = os.path.abspath(args.src)
    out_parent_dirname = source_dir + '_recortes'

    for root, _, files in os.walk(source_dir):
        for filename in files:
            if(filename.endswith('.csv')):
                out_dir = out_parent_dirname + '/' + os.path.basename(root).replace('_csv', '')
                out_dir += '' if out_dir[-1] == '/' else '/'
                csv_list.append((root + '/' + filename, out_dir))

    # if list is empty, exit
    list_size = len(csv_list)
    if(list_size > 0):
        print('Se encontr{} {} archivo{} CSV.'.format(
            'aron' if list_size != 1 else 'o', list_size, 's' if list_size != 1 else ''))
    else:
        print('No se encontraron archivos para procesar.')
        quit()

    cpu_count = list_size if list_size <= args.maxcpu else args.maxcpu
    if cpu_count > mp.cpu_count():
        cpu_count = mp.cpu_count()

    print('Procesos concurrentes: {}'.format(cpu_count))
    # if list is not empty, spawn process pool and begin
    with mp.Pool(cpu_count) as pool:
        pool.map(csvProcesser, csv_list)

    if args.minimum > 0:
        for entry in os.listdir(out_parent_dirname):
            target_folder = out_parent_dirname + '/' + entry
            n = len([f for f in os.listdir(target_folder)])
            if n < args.minimum:
                rmtree(target_folder)

    print('Listo.')


if __name__ == '__main__':
    main()
