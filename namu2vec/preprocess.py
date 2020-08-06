import json
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_file', type=str, default='../raw_data/namuwiki.json', help='input file path')
    parser.add_argument('--output_file', type=str, default='../raw_data/processed.txt', help='output file path')
    args = parser.parse_args()
    print('opening file' + args.input_file + '...')
    jsondata = json.load(open(args.input_file))
    print('successfully opened' + args.input_file)
    jsondata.pop(0) # namuwiki dump's first line is strange
    outfile = open(args.output_file, 'w')
    for doc in jsondata:
        line = doc['text'].replace('\n', '')
        toc_pos = line.find('목차')
        line = line[toc_pos+2:]
        tmp = outfile.write(line)
        tmp = outfile.write('\n')
