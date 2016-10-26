

filename = 'guitar_8_bit_mono.wav';
[y,Fs] = wavread(filename, 'native');
R = y(1:8000);

B = reshape(R,[],5);
csvwrite('data.txt',B);
[L,W] = size(R);


t=0:1/Fs:(length(y)-1)/Fs;
plot(t,y);


