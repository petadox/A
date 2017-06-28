#! /bin/bash
# -*- ENCODING: UTF-8 -*-

rm Misresultados.txt
var1=1
lim=10
while [ $var1 != $lim ]
do
   var2=$var1
   ((var2+=1))
   while [ $var2 != $lim ]
   do
	  echo "t$var1.txt t$var2.txt" > auxiliar.txt
      ./ej2 < auxiliar.txt >> Misresultados.txt
      echo -e "\n" >> Misresultados.txt
      rm auxiliar.txt
	  ((var2+=1))
   done
   ((var1+=1)) 
done
