# namu2vec
나무위키 문서와 facebook FastText를 이용해 한국어 임베딩을 구축하기 위한 전처리 코드입니다.
# Requirements
* Python interpreter
* tqdm (Python)
* soynlp (Python)
* C++ compiler
# Usage
## Json2txt
다운로드받은 JSON 파일에서 필요한 문서 정보만 TXT파일로 출력하는 코드
```
python3 json2txt.py [--txt_mode TXT_MODE] [--input_file INPUT_FILE_PATH] [--output_file OUTPUT_FILE_PATH]
```
* TXT_MODE: 데이터를 문장 단위로 나누어 학습시킬지 문서 단위로 나누어 학습시킬지 선택
* INPUT_FILE_PATH: 나무위키 문서 덤프 파일(namuwiki.json) 경로
* OUTPUT_FILE_PATH: 출력 파일 경로
## Decomposer
한글을 자모로 분리하는 코드
Python을 사용하고 싶었지만 파이썬 라이브러리 사용 시 전체 파싱에 1시간 이상이 걸리는 것을 확인하여 C++로 변경하였습니다.
```
g++ decomposer.cpp -std=c++17 -o decomposer
./decomposer INPUT_FILE_PATH OUTPUT_FILE_PATH
```
* INPUT_FILE_PATH: json2txt.py를 거친 파일 경로
* OUTPUT_FILE_PATH: 출력 파일 경로
# 문서 덤프 받기
나무위키 문서 덤프는 [공식 페이지](https://namu.wiki/w/%EB%82%98%EB%AC%B4%EC%9C%84%ED%82%A4:%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4%20%EB%8D%A4%ED%94%84)에서 받을 수 있습니다.

Currently under development.
