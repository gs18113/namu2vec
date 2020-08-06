import json
import argparse
from multiprocessing import Pool

from utils import jamo_sentence

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--mode', type=str, required=True, choices = ['json2txt', 'decompose'], help='processing mode')
    parser.add_argument('--txt_mode', type=str, default='sentence', choices = ['sentence', 'article'], help='processed input type, used for json2txt')
    parser.add_argument('--input_file', type=str, default='../raw_data/namuwiki.json', help='input file path')
    parser.add_argument('--output_file', type=str, default='../raw_data/processed.txt', help='output file path')
    parser.add_argument('--n_cpus', type=int, default=4, help='multiprocessing cpu count, used for decomposing')
    args = parser.parse_args()
    if args.mode == 'json2txt':
        print('opening file "' + args.input_file + '"...')
        jsondata = json.load(open(args.input_file))
        print('successfully opened file "' + args.input_file + '"')
        jsondata.pop(0) # namuwiki dump's first line is strange
        outfile = open(args.output_file, 'w')
        for doc in jsondata:
            line = doc['text'].replace('\n', '')
            toc_pos = line.find('목차')
            line = line[toc_pos+2:]
            if args.txt_mode == 'sentence':
                lines = line.split('.')
                for _line in lines:
                    if not _line:
                        continue
                    tmp = outfile.write(_line)
                    tmp = outfile.write('\n')
            else:
                tmp = outfile.write(line)
                tmp = outfile.write('\n')
    else:
        print('opening file "' + args.input_file + '"...')
        data = open(args.input_file).readlines()
        print('successfully opened file "' + args.input_file + '"')
        outfile = open(args.output_file, 'w')
        pool = Pool(args.n_cpus)
        jamo = pool.map(jamo_sentence,  data)
        for line in jamo:
            if not line:
                continue
            outfile.write(line)
            outfile.write('\n')