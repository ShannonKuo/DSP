#!/bin/bash
#./a.out -text testdata/1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/1.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/1.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/2.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/2.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/3.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/3.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/4.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/4.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/5.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/5.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/6.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/6.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/7.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/7.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/8.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/8.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/9.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/9.txt;
./srilm-1.5.10/bin/i686-m64/disambig -text testdata/10.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result1/10.txt;
