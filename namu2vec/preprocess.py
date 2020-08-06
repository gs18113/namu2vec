import json
import argparse
from soynlp.hangle import decompose
import re

doublespace_pattern = re.compile('\s+')

# function imported from https://lovit.github.io/nlp/representation/2018/10/22/fasttext_subword/
def jamo_sentence(sent):

    def transform(char):
        if char == ' ':
            return char
        cjj = decompose(char)
        if len(cjj) == 1:
            return cjj
        cjj_ = ''.join(c if c != ' ' else '-' for c in cjj)
        return cjj_

    sent_ = ''.join(transform(char) for char in sent)
    sent_ = doublespace_pattern.sub(' ', sent_)
    return sent_

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--mode', type=str, required=True, choices = ['json2txt', 'decompose'], help='processing mode')
    parser.add_argument('--txt_mode', type=str, default='sentence', choices = ['sentence', 'article'], help='processed input type')
    parser.add_argument('--input_file', type=str, default='../raw_data/namuwiki.json', help='input file path')
    parser.add_argument('--output_file', type=str, default='../raw_data/processed.txt', help='output file path')
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
                    tmp = outfile.write(_line)
                    tmp = outfile.write('\n')
            else:
                tmp = outfile.write(line)
                tmp = outfile.write('\n')
    else:
        print('opening file "' + args.input_file + '"...')
        data = open(args.input_file)
        print('successfully opened file "' + args.input_file + '"')
        outfile = open(args.output_file, 'w')
        while True:
            line = data.readline()
            if not line: 
                break
            outfile.write(jamo_sentence(line))