num_1 = 0;
num_2 = 0;
num_3 = 0;
num_4 = 0;

for i=1:max(size(HistTest))
   if (HistTest(i)==1)
       num_1 = num_1 + 1;
   end
   
   if (HistTest(i)==2)
       num_2 = num_2 + 1;
   end
   
   if (HistTest(i)==3)
       num_3 = num_3 + 1;
   end
   
   if (HistTest(i)==4)
       num_4 = num_4 + 1;
   end
end