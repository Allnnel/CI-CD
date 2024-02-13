#!/bin/bash
scp ./src/C3_SimpleBashUtils-5/src/cat/s21_cat ws1@10.0.3.10:/home/ws1
scp ./src/C3_SimpleBashUtils-5/src//grep/s21_grep ws1@10.0.3.10:/home/ws1
ssh ws1@10.0.3.10 'sudo mv /home/ws1/s21_cat /usr/local/bin; sudo mv /home/ws1/s21_grep /usr/local/bin'
