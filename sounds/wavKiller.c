#include <stdint.h>
#include <stdio.h>


FILE *fileptr;
FILE *outFILE;
char *buffer;
long filelen;







int main(int argc, char **argv){
	int i;
	for(i = 0;i<argc; i++){
		printf("%s\n", argv[i]);
	}

	unsigned char buffer[10];			// reading 10 bytes at a time?
	fileptr = fopen(argv[1], "rb");		// Gotta read binary
	fread(buffer, sizeof(buffer),1, fileptr);

	int j;
	for(j=0;j<10;j++){
		printf("%x ", buffer[j]);
	}

	
	
	unsigned int single_byte = 0;
	while(1){
		fread(&single_byte, sizeof(char), 1, fileptr);
		if(single_byte == 'd'){
			fread(&single_byte, sizeof(char), 1, fileptr);
			if(single_byte == 'a'){
				fread(&single_byte, sizeof(char), 1, fileptr);
				if(single_byte == 't'){
					fread(&single_byte, sizeof(char), 1, fileptr);
					if(single_byte == 'a'){
						break;
					}
				}
			}
		}
	}


	outFILE = fopen(argv[2], "w");
	if(outFILE==NULL){
		printf("bad file\n\r");
		return;
	}

	fprintf(outFILE, "START of our awesome file:\n\n");

	size_t yeah;
	while((yeah = fread(&single_byte, sizeof(char), 1, fileptr)) != 0){
		fprintf(outFILE, "%u, ", single_byte);
	}

	fclose(outFILE);
	fclose(fileptr);
	
/*	fileptr = fopen(argv[0], "rb");
	fseek(fileptr, 0, SEEK_END);

	filelen = ftell(fileptr);
	rewind(fileptr);

	buffer = (char *) malloc((filelen+1) * sizeof(char));
	fread(buffer, filelen, 1, fileptr);
	fclose(fileptr);
*/	
	
	return 0;
}
