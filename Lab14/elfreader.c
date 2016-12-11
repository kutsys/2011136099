#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <elf.h>

#define MAX_PATH 256
#define TRUE 1
#define FALSE 0

int check_elf(Elf32_Ehdr ehdr) {
    /* ELF Magic Number 0x7f, 'E', 'L', 'F' : 총 4바이트 */
    if(!strncmp((char *)ehdr.e_ident, "\177ELF", 4)) {
        printf("ELF파일이 맞습니다.\n");
        return TRUE;
    }
    else {
        printf("ELF파일이 아닙니다.\n");
        return FALSE;
    }
}

void print_ELF_Header(Elf32_Ehdr ehdr) {
    printf("\t\t\t*****ELF HEADER*****\n");
    printf("\t\t-ELF identification-\n");
    printf("\t1. Magic Number: %c%c%c%c\n", ehdr.e_ident[EI_MAG0], ehdr.e_ident[EI_MAG1], ehdr.e_ident[EI_MAG2], ehdr.e_ident[EI_MAG3]);
    
    printf("\t2. File Class: ");
    switch(ehdr.e_ident[EI_CLASS]) {
        case ELFCLASS32:
            printf("32-bit objects\n");
            break;
        case ELFCLASS64:
            printf("64-bit objects\n");
            break;
        default:
            printf("Invalid class\n");
    }

    printf("\t3. DATA Encoding: ");
    switch(ehdr.e_ident[EI_DATA]) {
        case ELFDATA2LSB:
            printf("little endian\n");
            break;
        case ELFDATA2MSB:
            printf("big endian\n");
            break;
        default:
            printf("Invalid data encoding\n");
    }

    printf("\t4. ELF header version: %d\n", ehdr.e_ident[EI_VERSION]);

    printf("\t5. ELF file type: ");
    switch(ehdr.e_type) {
        case ET_NONE:
            printf("No file type\n");
            break;
        case ET_REL:
            printf("Relocatable file\n");
            break;
        case ET_EXEC:
            printf("Executable file\n");
            break;
        case ET_DYN:
            printf("Shared object file\n");
            break;
        case ET_CORE:
            printf("Core file\n");
            break;
        case ET_LOPROC:
            printf("Processor-specific\n");
            break;
        case ET_HIPROC:
            printf("Processor-specific\n");
            break;
        default:
            printf("Unknown (0x%x)\n", ehdr.e_type);
    }

    printf("\t6. Processor Architecture: ");
    switch(ehdr.e_machine) {
        case EM_NONE:
            printf("No machine\n");
            break;
        case EM_M32:
            printf("At&T WE 32100\n");
            break;
        case EM_SPARC:
            printf("SPARC\n");
            break;
        case EM_386:
            printf("Intel 80386\n");
            break;
        case EM_68K:
            printf("Motorola 68000\n");
            break;
        case EM_88K:
            printf("Motorola 88000\n");
            break;
        case EM_860:
            printf("Intel 80860\n");
            break;
        case EM_MIPS:
            printf("MIPS RS3000\n");
            break;
        default:
            printf("Unknown (0x%x)\n", ehdr.e_machine);
    }

    printf("\t7. ELF file version: ");
    switch(ehdr.e_version) {
        case EV_NONE:
            printf("Invalid version\n");
            break;
        case EV_CURRENT:
            printf("Current version\n");
            break;
        default:
            printf("Unknown (0x%x)\n", ehdr.e_version);
    }

    printf("\t8. 실행시작 가상 어드레스: (0x%x)\n", ehdr.e_entry);

    printf("\t9. Program Header Table(PHT)\n");
    printf("\t\tPHT의 file offset: (0x%x)\n", ehdr.e_phoff);
    printf("\t\tPHT의 entry 크기: %d\n", ehdr.e_phentsize);
    printf("\t\tPHT의 entry 개수: %d\n", ehdr.e_phnum);

    printf("\t10. Section Header Table(SHT)\n");
    printf("\t\tSHT의 file offset: (0x%x)\n", ehdr.e_shoff);
    printf("\t\tSHT의 entry 크기: %d\n", ehdr.e_shentsize);
    printf("\t\tSHT의 entry 개수: %d\n", ehdr.e_shnum);
    printf("\t\tstring table의 SHT index: %d\n", ehdr.e_shstrndx);

    printf("\t11. processor-specific flag: (0x%x)\n", ehdr.e_flags);

    printf("\t\t\t***************\n");
}

int main(int argc, char **argv) {
    int opt;
    int fd;
    Elf32_Ehdr ehdr;    // ELF-Header, 크기 고정
    Elf32_Shdr* shdr;    // Section-Header, 크기 가변
    char file_path[MAX_PATH];
    char option[4] = {'\0',};
    char buf[256];
    int i = 0;
    int elf_header_opt = 0;
    int num_of_opt = 0;

    while((opt = getopt(argc, argv, "eps")) != -1) {
        switch(opt) {
            case 'e':
                elf_header_opt = 1;
                break; 
            case 'p':
                option[i] = 'l';
                i++;
                break;
            case 's':
                option[i] = 'S';
                i++;
                break;
            default:
                printf("%c는 알수없는 옵션 입니다.\n", opt);
                break;
        }
        num_of_opt++;
    }

    if(num_of_opt == 0) {
        elf_header_opt = 1;
    }

    for(; optind < argc; optind++) {
        strcpy(file_path, argv[optind]);
    }

    fd = open(file_path, O_RDONLY|O_SYNC);
    if(fd < 0) {
        fprintf(stderr, "Can't open %s.\n", file_path);
    }

    /* ELF-Header를 읽는다. */
    if(read(fd, (void *)&ehdr, sizeof(Elf32_Ehdr)) == -1) {
        fprintf(stderr, "ELF Header를 읽지 못했습니다.\n");
        exit(EXIT_FAILURE);
    }

    if(check_elf(ehdr) == FALSE) {
        exit(EXIT_FAILURE);
    }

    if (elf_header_opt == 1) {
        // ELF HEADER 출력
        print_ELF_Header(ehdr); 
    }

    if(option[0] != '\0') {
        sprintf(buf, "readelf -%s %s",option ,file_path);
        system(buf);
     }

    exit(EXIT_SUCCESS);
}
