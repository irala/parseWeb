#!/bin/bash
#script to generate a new web to parse

#usage ./get_web.sh https://www.noticias3d.com
curl $1 > html
elinks -dump html > html.txt