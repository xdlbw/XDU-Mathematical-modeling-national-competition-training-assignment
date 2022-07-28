data example1;
input x@@;
time= n;
cards;
126.4   82.4    78.1    51.1    90.9    76.2    104.5   87.4
110.5   25      69.3    53.5    39.8    63.6    46.7    72.9
79.6    83.6    80.7    60.3    79      74.4    49.6    54.7    
71.8    49.1    103.9   51.6    82.4    83.6    77.8    79.3
89.6    85.5    58      120.7   110.5   65.4    39.9    40.1
88.7    71.4    83      55.9    89.9    84.8    105.2   113.7
124.7   114.5   115.6   102.4   101.4   89.8    71.5    70.9
98.3    55.5    66.1    78.4    120.5   97      110
;
proc gplot data=example1;
plot x*time=1;
symbol1 c=red i=join v=star;
proc arima data=example1;
identify var=x nlag=8 minic p=(0:5) q=(0:5);
estimate p=1;
forecast lead=5 id=time out=results;
run;