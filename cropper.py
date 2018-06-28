#Python Cropper Tool
import os
import csv
import cv2
import math
import argparse
import multiprocessing as mp

parser = argparse.ArgumentParser()
parser.add_argument("src", help="carpeta donde se encuentran las imagenes de origen", type=str)
parser.add_argument("--max-cpu", "-m", metavar="M", action="store", dest="maxcpu", help="numero maximo de procesos permitidos", type=int, default=mp.cpu_count())
parser.add_argument("--quality", "-q", metavar="Q", action="store", dest="quality", help="calidad de los recortes", type=str, default="")
parser.add_argument("--tag", "-t", metavar="T", action="store", dest="tag", help="etiquetas de los recortes", type=str, default="")
args = parser.parse_args()

source_dir = os.path.abspath(args.src)
source_name = os.path.basename(source_dir)
parent_dir = os.path.abspath(os.path.join(source_dir, os.pardir))

output_dir = parent_dir + "/" + source_name + "_recortes"
csv_dir = parent_dir + "/" + source_name + "_csv"

if args.maxcpu <= 0:
    print("Numero de CPUs no valido.")
    quit()

def csvProcesser(file_uri):
    with open(file_uri, 'rt') as csvfile:
        csvdata = csv.reader(csvfile, delimiter=',')
        crop_count = 1
        img_uri = file_uri[:-4].replace('_csv', '')
        
        slashpos = file_uri.rfind("/")
        stol_dot = file_uri[:-4].rfind(".")
        img_name = file_uri[slashpos+1:stol_dot]
        img_extension = file_uri[stol_dot:-4]

        for row in csvdata:
            #skip crop if it doesn't match specified tags and/or quality (if any)
            if args.tag != "":
                if args.tag.upper() != row[4].upper():
                    return
            if args.quality != "":
                if args.quality.upper() != row[5].upper():
                    return
            x1 = int(row[0])
            y1 = int(row[1])
            x2 = int(row[2])
            y2 = int(row[3])
            #open image, crop, save, close csv
            img = cv2.imread(img_uri)
            w = x2 - x1
            h = y2 - y1

            if w > h:
                padding = (w - h)/2
                upper_padding = math.ceil(padding)
                lower_padding = math.floor(padding)
                target_crop = cv2.copyMakeBorder(img, upper_padding, lower_padding, 0, 0, cv2.BORDER_CONSTANT, None, [0,0,0])
            elif h > w:
                padding = (h - w)/2
                left_padding = math.ceil(padding)
                right_padding = math.floor(padding)
                target_crop = cv2.copyMakeBorder(img, 0, 0, left_padding, right_padding, cv2.BORDER_CONSTANT, None, [0,0,0])
            else:
                target_crop = img[y1:y2, x1:x2]

            dest_file = output_dir+"/"+img_name+"_"+str(crop_count).zfill(3)+img_extension
            crop_count+=1
            cv2.imwrite(dest_file, target_crop)
            del target_crop
        csvfile.close()


#loop over files in source dir, put CSV files in list
csv_bag = []
for file in os.listdir(csv_dir):
    filename = os.fsdecode(file)
    if(filename.endswith(".csv")):
        csv_bag.append(csv_dir + "/" + filename)

#if list is empty, exit
list_size = len(csv_bag)
if(list_size > 0):
    print("Se %s %i %s CSV."%("encontraron" if list_size > 1 else "encontro", 
        list_size, "archivos" if list_size > 1 else "archivo"))
else:
    print("No se encontraron archivos para procesar.")
    quit()

#create output folder if it doesn't exist
os.makedirs(os.path.dirname(output_dir +"/"), exist_ok=True)

manager = mp.Manager()
folder_dict = manager.dict()

cpu_count = list_size if list_size <= args.maxcpu else args.maxcpu
if cpu_count > mp.cpu_count():
    cpu_count = mp.cpu_count()

print("Procesos concurrentes: %i"%(cpu_count))
#if list is not empty, spawn process pool and begin
with mp.Pool(cpu_count) as pool:
    pool.map(csvProcesser, csv_bag)
