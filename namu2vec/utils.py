from soynlp.hangle import decompose
import re

doublespace_pattern = re.compile('\s+')

# function imported from https://lovit.github.io/nlp/representation/2018/10/22/fasttext_subword/
def jamo_sentence(sent):

    def transform(char):
        if char == ' ':
            return char
        cjj = decompose(char)
        if not cjj:
            return ''
        if len(cjj) == 1:
            return cjj
        cjj_ = ''.join(c if c != ' ' else '-' for c in cjj)
        return cjj_

    sent_ = ''.join(transform(char) for char in sent)
    sent_ = doublespace_pattern.sub(' ', sent_)
    return sent_
