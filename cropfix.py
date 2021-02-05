#!/usr/bin/env python
# Python Crop Fixer
import os
import csv
import cv2
import math
import argparse
import numpy as np
import multiprocessing as mp
from shutil import move

parser = argparse.ArgumentParser()
parser.add_argument('src', help='carpeta donde se encuentran las imagenes de origen', type=str)
parser.add_argument('--max-cpu', '-m', action='store', dest='maxcpu', help='numero maximo de procesos permitidos', type=int, default=mp.cpu_count())
args = parser.parse_args()


def translatePoint(point: tuple, rotations: int, dimensions: tuple):
    x, y = point
    height, width = dimensions
    if rotations == 0:
        return (x, y)
    elif rotations == 1:
        return (width - y, x)
    elif rotations == 2:
        return (width - x, height - y)
    elif rotations == 3:
        return (y, height - x)


def csvProcesser(filepath: str):

    with open(filepath, 'rt') as csvfile:
        csvdata = csv.reader(csvfile, delimiter=',')
        crop_count = 1
        img_path = filepath[:-4].replace('_csv', '')

        slashpos = filepath.rfind('/')
        stol_dot = filepath[:-4].rfind('.')
        img_name = filepath[slashpos+1:stol_dot]
        img_extension = filepath[stol_dot:-4]

        img = cv2.imread(img_path)

        # work out necessary rotations
        rotations = 0

        br_pixel = None  # bottom right pixel
        bl_pixel = None  # bottom left pixel
        tr_pixel = None  # top rireturn (width - y, x)ght pixel
        tl_pixel = None  # top left pixel

        img_h, img_w, _ = img.shape

        for row in csvdata:
            if row[0][0] == '#':
                rotations = int(int(row[0][1:])/90)
            elif row[0][0] == '$':
                if br_pixel is None:
                    br_pixel = tuple(int(x) for x in row[0][1:].split(':'))
                elif bl_pixel is None:
                    tl_pixel = tuple(int(x) for x in row[0][1:].split(':'))
                elif tr_pixel is None:
                    tl_pixel = tuple(int(x) for x in row[0][1:].split(':'))
                elif tl_pixel is None:
                    tl_pixel = tuple(int(x) for x in row[0][1:].split(':'))

        if br_pixel is None:
            return

        br_pixel2 = tuple([img[img_h - 1, img_w - 1, 0], img[img_h - 1, img_w - 1, 1], img[img_h - 1, img_w - 1, 2]])
        bl_pixel2 = tuple([img[img_h - 1, 0, 0], img[img_h - 1, 0, 1], img[img_h - 1, 0, 2]])
        tr_pixel2 = tuple([img[0, img_w - 1, 0], img[0, img_w - 1, 1], img[0, img_w - 1, 2]])
        tl_pixel2 = tuple([img[0, 0, 0], img[0, 0, 1], img[0, 0, 2]])

        spins = 0

        # this rotation is to leave the image at the same angle that the cropper tool thinks it is in
        while br_pixel != br_pixel2 and tl_pixel != tl_pixel2 and bl_pixel != bl_pixel2 and tr_pixel != tr_pixel2:
            spins += 1
            img = cv2.flip(cv2.transpose(img), 1)
            img_h, img_w, _ = img.shape
            br_pixel2 = tuple([img[img_h - 1, img_w - 1, 0], img[img_h - 1, img_w - 1, 1], img[img_h - 1, img_w - 1, 2]])
            bl_pixel2 = tuple([img[img_h - 1, 0, 0], img[img_h - 1, 0, 1], img[img_h - 1, 0, 2]])
            tr_pixel2 = tuple([img[0, img_w - 1, 0], img[0, img_w - 1, 1], img[0, img_w - 1, 2]])
            tl_pixel2 = tuple([img[0, 0, 0], img[0, 0, 1], img[0, 0, 2]])

        if spins <= 0:
            return
        print(spins, '->', filepath)
        #return

        # this rotation is the actual rotation done by the user in the cropper tool
        for _ in range(rotations):
            img = cv2.flip(cv2.transpose(img), 1)

        final_rotations = (spins + 2) % 4

        img = cv2.imread(img_path)
        print(final_rotations)

        # process crops
        csvfile.seek(0)
        temp_file = open(filepath + '.tmp', 'wt')
        translated_csv = csv.writer(temp_file, quoting=csv.QUOTE_NONE)
        for row in csvdata:
            if row[0][0] == '$':
                continue
            if row[0][0] == '#' or row[0][0] == '$' or row[0][0] == '%':
                translated_csv.writerow(row)
                continue
            x1 = int(row[0])
            y1 = int(row[1])
            x2 = int(row[2])
            y2 = int(row[3])

            point_a = (x1, y1)
            point_b = (x2, y1)
            point_c = (x2, y2)
            point_d = (x1, y2)

            if final_rotations == 0:
                return
            elif final_rotations == 1:
                x1, y1 = translatePoint(point_d, final_rotations, img.shape[:2])
                x2, y2 = translatePoint(point_b, final_rotations, img.shape[:2])
            elif final_rotations == 2:
                x1, y1 = translatePoint(point_c, final_rotations, img.shape[:2])
                x2, y2 = translatePoint(point_a, final_rotations, img.shape[:2])
            elif final_rotations == 3:
                x1, y1 = translatePoint(point_b, final_rotations, img.shape[:2])
                x2, y2 = translatePoint(point_d, final_rotations, img.shape[:2])
            translated_csv.writerow([x1, y1, x2, y2, row[4], row[5]])
        csvfile.close()
        temp_file.close()
        move(filepath + '.tmp', filepath)


def main():
    if args.maxcpu <= 0:
        print('Numero de CPUs no valido.')
        quit()
    # loop over files in source dir, put CSV files in list
    csv_list = []

    source_dir = os.path.abspath(args.src)

    for root, _, files in os.walk(source_dir):
        for filename in files:
            if(filename.endswith('.csv')):
                csv_list.append(root + '/' + filename)

    # if list is empty, exit
    list_size = len(csv_list)
    if(list_size > 0):
        print('Se encontr{} {} archivo{} CSV.'.format('aron' if list_size != 1 else 'o', list_size, 's' if list_size != 1 else ''))
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

    print('Listo.')


if __name__ == '__main__':
    main()
