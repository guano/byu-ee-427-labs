

filename = 'fastinvader3.wav';
[y,Fs] = wavread(filename, 'native');
R = y(1:1040); 
B = reshape(R,[],10);
csvwrite('data.txt',B,0,1);
[L,W] = size(R);


t=0:1/Fs:(length(y)-1)/Fs;
plot(t,y);


