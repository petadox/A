#! /bin/bash
# -*- ENCODING: UTF-8 -*-

rm Misresultados.txt
var1=1
lim=11
aux=1
while [ $var1 != $lim ]
do
   var2=$var1
   ((var2+=1))
   while [ $var2 != $lim ]
   do
	  echo "test$var1.txt test$var2.txt" > auxiliar.txt
      ./ej2 < auxiliar.txt >> Misresultados.txt
      echo -e "\n" >> Misresultados.txt
      echo $aux
      rm auxiliar.txt
	  ((var2+=1))
   done
   ((var1+=1)) 
done
