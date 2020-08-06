#include<stdio.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

const int chosung[] = {0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314a, 0x314b, 0x314c, 0x314d, 0x314e};
const int jungsung[] = {0x314f, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315a, 0x315b, 0x315c, 0x315d, 0x315e, 0x315f, 0x3160, 0x3161, 0x3162, 0x3163};
const int jongsung[] = {0x0000, 0x3131, 0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137, 0x3139, 0x313a, 0x313b, 0x313c, 0x313d, 0x313e, 0x313f, 0x3140, 0x3141, 0x3142, 0x3144, 0x3145, 0x3146, 0x3147, 0x3148, 0x314a, 0x314b, 0x314c, 0x314d, 0x314e};

static int mask1 = (1<<16) - (1<<12);
static int mask2 = (1<<12) - (1<<6);
static int mask3 = (1<<6) - 1;

inline void write_to_buf(char* out_buf, int &cur_out_buf_size, int &complete_code){
	if(0x3131 <= complete_code && complete_code <= 0x318E){ // hangul compatibility jamo
		out_buf[cur_out_buf_size++] = (7<<5)+((complete_code&mask1)>>12);
		out_buf[cur_out_buf_size++] = (1<<7)+((complete_code&mask2)>>6);
		out_buf[cur_out_buf_size++] = (1<<7)+((complete_code&mask3));
	}
	else if(0xAC00 <= complete_code && complete_code <= 0xD7A3){ // hangul syllables
		int index = complete_code - 0xAC00;
		int chosung_id = chosung[index / (21 * 28)];
		int jungsung_id = jungsung[(index - chosung_id * 21 * 28) / 28];
		int jongsung_id = jongsung[(index - chosung_id * 21 * 28 - jungsung_id * 28)];
		// chosung
		out_buf[cur_out_buf_size++] = (7<<5)+((chosung_id&mask1)>>12);
		out_buf[cur_out_buf_size++] = (1<<7)+((chosung_id&mask2)>>6);
		out_buf[cur_out_buf_size++] = (1<<7)+((chosung_id&mask3));

		// jungsung
		out_buf[cur_out_buf_size++] = (7<<5)+((jungsung_id&mask1)>>12);
		out_buf[cur_out_buf_size++] = (1<<7)+((jungsung_id&mask2)>>6);
		out_buf[cur_out_buf_size++] = (1<<7)+((jungsung_id&mask3));
		
		// jongsung
		out_buf[cur_out_buf_size++] = (7<<5)+((jongsung_id&mask1)>>12);
		out_buf[cur_out_buf_size++] = (1<<7)+((jongsung_id&mask2)>>6);
		out_buf[cur_out_buf_size++] = (1<<7)+((jongsung_id&mask3));
	}
	else if(complete_code == '\n'){
		out_buf[cur_out_buf_size++] = '\n';
	}
	else{
		if(out_buf[cur_out_buf_size-1] != ' ') out_buf[cur_out_buf_size++] = ' ';
	}
}

static int decompose(char const *infile, char const *outfile) {
	static const auto BUFFER_SIZE = 16*1024;
	static const auto OUT_BUFFER_SIZE = 16*1024*3;
	char out_buf[OUT_BUFFER_SIZE + 1];
	int cur_out_buf_size;
	int fin = open(infile, O_RDONLY);
	if(fin == -1){
		printf("open failed!");
		return -1;
	}

	/* Advise the kernel of our access pattern.  */
	posix_fadvise(fin, 0, 0, 1);  // FDADVICE_SEQUENTIAL

	char buf[BUFFER_SIZE + 1];

	FILE* fout = fopen(outfile, "wb" );

	while(size_t bytes_read = read(fin, buf, BUFFER_SIZE))
	{
		if(bytes_read == (size_t)-1){
			printf("read failed!");
			return -1;
		}

		if (!bytes_read)
			break;
		int partial_code = -1;
		int complete_code = 0;
		for(char *p = buf; p < buf + bytes_read; ++p){
			if((*p)&(1<<7) != 0){
				if((*p)&(1<<6) != 0){
					if((*p)&(1<<5) != 0){
						if((*p)&(1<<4) != 0){
							complete_code = partial_code;
							partial_code = (*p)&((1<<3)-1);
						}
						else{
							complete_code = partial_code;
							partial_code = (*p)&((1<<4)-1);
						}
					}
					else{
						complete_code = partial_code;
						partial_code = (*p)&((1<<5)-1);
					}
				}
				else{
					partial_code <<= 6;
					partial_code += (*p)&((1<<6)-1);
				}
			}
			else{
				complete_code = partial_code;
				partial_code = *p;
			}
			if(!((*p)&(3<<6)==(1<<7))&&complete_code!=-1){
				write_to_buf(out_buf, cur_out_buf_size, complete_code);
			}
		}
		complete_code = partial_code;
		if(complete_code!=-1){
			write_to_buf(out_buf, cur_out_buf_size, complete_code);
		}
		fwrite(out_buf, 1, cur_out_buf_size, fout);
	}

	return 1;
}

int main(int argc, char *argv[]){
	if(argc <= 2){
		printf("Invalid input. Usage: ./decomposer <input file path> <output file path>\n");
	}
	decompose(argv[1], argv[2]);
	return 0;
}
