#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// ANSI Color Codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"

#define SECTOR_SIZE 2048
#define ISO_FILENAME_LENGTH 32

typedef struct {
	char name[ISO_FILENAME_LENGTH];
	unsigned int size;
	unsigned int lba;
} IsoFile;

typedef struct {
	char name[ISO_FILENAME_LENGTH];
	unsigned int size;
	unsigned int lba;
	int num_files;
	IsoFile *files;
} IsoDirectory;

void print_banner(void)
{
	printf("\n%s%s╔══════════════════════════════════════╗%s\n", BOLD, CYAN,
	       RESET);
	printf("%s%s║          ISOMake v1.0                ║%s\n", BOLD, CYAN,
	       RESET);
	printf("%s%s║  ISO 9660 Image Creation Utility     ║%s\n", BOLD, CYAN,
	       RESET);
    printf("%s%s║   Developed By VoltagedDebunked      ║%s\n", BOLD, CYAN,
	       RESET);
	printf("%s%s╚══════════════════════════════════════╝%s\n\n", BOLD, CYAN,
	       RESET);
}

void print_progress(const char *message, int success)
{
	if (success) {
		printf("%s%s[✓]%s %s\n", BOLD, GREEN, RESET, message);
	} else {
		printf("%s%s[✗]%s %s\n", BOLD, RED, RESET, message);
	}
}

void write_volume_descriptor(FILE *iso_file)
{
	char descriptor[SECTOR_SIZE] = { 0 };
	descriptor[0] = 1; // Volume Descriptor Type
	memcpy(descriptor + 1, "CD001", 5); // Standard Identifier
	descriptor[6] = 1; // Volume Descriptor Version

	time_t now = time(NULL);
	struct tm *tm_info = gmtime(&now);
	sprintf(descriptor + 190, "%04d%02d%02d%02d%02d%02d00",
		tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
		tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

	fwrite(descriptor, 1, SECTOR_SIZE, iso_file);
	print_progress("Volume descriptor written", 1);
}

void write_directory_record(FILE *iso_file, const char *name, unsigned int lba,
			    unsigned int size)
{
	unsigned char record[34] = { 0 };
	int name_len = strlen(name);

	record[0] = 33 + name_len; // Length of Directory Record
	record[2] = lba & 0xFF;
	record[3] = (lba >> 8) & 0xFF;
	record[4] = (lba >> 16) & 0xFF;
	record[5] = (lba >> 24) & 0xFF;
	record[10] = size & 0xFF;
	record[11] = (size >> 8) & 0xFF;
	record[12] = (size >> 16) & 0xFF;
	record[13] = (size >> 24) & 0xFF;
	record[32] = name_len;

	fwrite(record, 1, 33, iso_file);
	fwrite(name, 1, name_len, iso_file);
}

void add_file_to_iso(FILE *iso_file, const char *filepath, unsigned int *lba)
{
	FILE *input_file = fopen(filepath, "rb");
	if (!input_file) {
		printf("%s%sError:%s Unable to open file: %s%s%s\n", BOLD, RED,
		       RESET, YELLOW, filepath, RESET);
		return;
	}

	fseek(input_file, 0, SEEK_END);
	long file_size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);

	char *buffer = malloc(file_size);
	fread(buffer, 1, file_size, input_file);
	fclose(input_file);

	fseek(iso_file, (*lba) * SECTOR_SIZE, SEEK_SET);
	fwrite(buffer, 1, file_size, iso_file);
	free(buffer);

	const char *filename = strrchr(filepath, '/');
	if (filename) {
		filename++;
	} else {
		filename = filepath;
	}

	write_directory_record(iso_file, filename, *lba, file_size);
	printf("%s%s[+]%s Added file: %s%s%s (Size: %s%.2f MB%s)\n", BOLD, BLUE,
	       RESET, GREEN, filename, RESET, YELLOW,
	       (float)file_size / (1024 * 1024), RESET);

	*lba += (file_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
}

void print_usage(const char *program_name)
{
	printf("%s%sUsage:%s %s <output.iso> <input_file1> [input_file2] ...\n",
	       BOLD, YELLOW, RESET, program_name);
	printf("%s%sExample:%s %s output.iso file1.txt file2.jpg\n\n", BOLD,
	       YELLOW, RESET, program_name);
}

int main(int argc, char *argv[])
{
	print_banner();

	if (argc < 3) {
		print_usage(argv[0]);
		return 1;
	}

	printf("%s%s[*]%s Creating ISO image: %s%s%s\n", BOLD, MAGENTA, RESET,
	       CYAN, argv[1], RESET);

	FILE *iso_file = fopen(argv[1], "wb");
	if (!iso_file) {
		printf("%s%sError:%s Failed to create ISO file\n", BOLD, RED,
		       RESET);
		return 1;
	}

	// Write 16 empty sectors
	char empty_sector[SECTOR_SIZE] = { 0 };
	for (int i = 0; i < 16; i++) {
		fwrite(empty_sector, 1, SECTOR_SIZE, iso_file);
	}
	print_progress("Empty sectors written", 1);

	// Write Primary Volume Descriptor
	write_volume_descriptor(iso_file);

	// Write root directory
	unsigned int root_lba = 18;
	fseek(iso_file, root_lba * SECTOR_SIZE, SEEK_SET);

	write_directory_record(iso_file, "\0", root_lba, SECTOR_SIZE);
	write_directory_record(iso_file, "\1", root_lba, SECTOR_SIZE);
	print_progress("Root directory initialized", 1);

	unsigned int current_lba = root_lba + 1;

	// Add files to ISO
	printf("\n%s%s[*]%s Adding files to ISO image:\n", BOLD, MAGENTA,
	       RESET);
	for (int i = 2; i < argc; i++) {
		add_file_to_iso(iso_file, argv[i], &current_lba);
	}

	fclose(iso_file);
	printf("\n%s%s[✓]%s ISO image created successfully: %s%s%s\n", BOLD,
	       GREEN, RESET, CYAN, argv[1], RESET);

	return 0;
}