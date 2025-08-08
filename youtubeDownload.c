
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_URL_LENGTH 2048
#define MAX_COMMAND_LENGTH 4096
#define MAX_FILENAME_LENGTH 512

// Function to check if yt-dlp is installed
int check_ytdlp_installed() {
	int status = system("yt-dlp --version > /dev/null 2>&1");
	return (status == 0);
}

// Function to validate YouTube URL
int is_valid_youtube_url(const char* url) {
	if (url == NULL || strlen(url) == 0) {
		return 0;
	}
	
	// Check for common YouTube URL patterns
	if (strstr(url, "youtube.com/watch") != NULL ||
		strstr(url, "youtu.be/") != NULL ||
		strstr(url, "youtube.com/playlist") != NULL ||
		strstr(url, "youtube.com/channel") != NULL) {
			return 1;
		}
	
	return 0;
}

// Function to sanitize filename (remove potentially dangerous characters)
void sanitize_filename(char* filename) {
	char* dangerous_chars = "\\/:*?\"<>|";
	for (int i = 0; filename[i]; i++) {
		for (int j = 0; dangerous_chars[j]; j++) {
			if (filename[i] == dangerous_chars[j]) {
				filename[i] = '_';
				break;
			}
		}
	}
}

// Function to download YouTube video
int download_youtube_video(const char* url, const char* output_path, const char* quality) {
	char command[MAX_COMMAND_LENGTH];
	int status;
	
	printf("Starting download...\n");
	printf("URL: %s\n", url);
	printf("Output path: %s\n", output_path ? output_path : "current directory");
	printf("Quality: %s\n", quality);
	printf("=========================================\n");
	
	// Build the yt-dlp command
	if (output_path && strlen(output_path) > 0) {
		snprintf(command, sizeof(command),
			"yt-dlp -f \"%s\" --merge-output-format mp4 -o \"%s/%%(title)s.%%(ext)s\" \"%s\"",
			quality, output_path, url);
	} else {
		snprintf(command, sizeof(command),
			"yt-dlp -f \"%s\" --merge-output-format mp4 -o \"%%(title)s.%%(ext)s\" \"%s\"",
			quality, url);
	}
	
	printf("Executing: %s\n", command);
	printf("=========================================\n");
	
	// Execute the command
	status = system(command);
	
	if (status == 0) {
		printf("=========================================\n");
		printf("Download completed successfully!✅\n");
		return 1;
	} else {
		printf("=========================================\n");
		printf("Download failed with status:❌ %d\n", status);
		return 0;
	}
}

// Function to list available formats
void list_formats(const char* url) {
	char command[MAX_COMMAND_LENGTH];
	
	printf("Available formats for this video:📺\n");
	printf("=========================================\n");
	
	snprintf(command, sizeof(command), "yt-dlp -F \"%s\"", url);
	system(command);
	
	printf("=========================================\n");
}

// Function to get video info
void get_video_info(const char* url) {
	char command[MAX_COMMAND_LENGTH];
	
	printf("Video information:\n");
	printf("=========================================\n");
	
	snprintf(command, sizeof(command), 
		"yt-dlp --get-title --get-duration --get-description \"%s\" | head -20", 
		url);
	system(command);
	
	printf("=========================================\n");
}

void print_usage(const char* program_name) {
	printf("YouTube Video Downloader\n");
	printf("By: @planetminguez\n\n");
	printf("Usage: %s [OPTIONS] <YouTube_URL>\n\n", program_name);
	printf("Options:\n");
	printf("  -h, --help              Show this help message\n");
	printf("  -o, --output <path>     Output directory (default: current directory)\n");
	printf("  -q, --quality <format>  Video quality/format (default: best[ext=mp4])\n");
	printf("  -l, --list-formats      List available formats for the video\n");
	printf("  -i, --info              Show video information\n\n");
	printf("Quality options:\n");
	printf("  best[ext=mp4]          Best quality MP4 (default)\n");
	printf("  worst[ext=mp4]         Worst quality MP4\n");
	printf("  720p[ext=mp4]          720p MP4\n");
	printf("  480p[ext=mp4]          480p MP4\n");
	printf("  360p[ext=mp4]          360p MP4\n");
	printf("  bestvideo+bestaudio    Best video + best audio (requires ffmpeg)\n\n");
	printf("Examples:\n");
	printf("  %s https://www.youtube.com/watch?v=dQw4w9WgXcQ\n", program_name);
	printf("  %s -o ~/Downloads -q 720p https://youtu.be/dQw4w9WgXcQ\n", program_name);
	printf("  %s -l https://www.youtube.com/watch?v=dQw4w9WgXcQ\n", program_name);
}

int main(int argc, char* argv[]) {
	char url[MAX_URL_LENGTH] = {0};
	char output_path[MAX_FILENAME_LENGTH] = {0};
	char quality[256] = "best[ext=mp4]";  // Default quality
	int list_formats_flag = 0;
	int show_info_flag = 0;
	
	// Check if yt-dlp is installed
	if (!check_ytdlp_installed()) {
		printf("Error: yt-dlp is not installed or not in PATH.\n");
		printf("Please install yt-dlp first:\n");
		printf("  pip install yt-dlp\n");
		printf("  or visit: https://github.com/yt-dlp/yt-dlp\n");
		return 1;
	}
	
	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			print_usage(argv[0]);
			return 0;
		}
		else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			if (i + 1 < argc) {
				strncpy(output_path, argv[++i], sizeof(output_path) - 1);
			} else {
				printf("Error: Output path not specified.\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quality") == 0) {
			if (i + 1 < argc) {
				strncpy(quality, argv[++i], sizeof(quality) - 1);
			} else {
				printf("Error: Quality not specified.\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list-formats") == 0) {
			list_formats_flag = 1;
		}
		else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) {
			show_info_flag = 1;
		}
		else if (argv[i][0] != '-') {
			// This should be the URL
			strncpy(url, argv[i], sizeof(url) - 1);
		}
	}
	
	// Check if URL was provided
	if (strlen(url) == 0) {
		printf("\n");
		printf("Error: No YouTube URL provided.\n");
		printf("\n\n");
		print_usage(argv[0]);
		return 1;
	}
	
	// Validate YouTube URL
	if (!is_valid_youtube_url(url)) {
		printf("Error: Invalid YouTube URL.\n");
		printf("Please provide a valid YouTube URL (youtube.com or youtu.be)\n");
		return 1;
	}
	
	printf("YouTube Video Downloader\n");
	printf("========================\n\n");
	
	// Handle different modes
	if (show_info_flag) {
		get_video_info(url);
		return 0;
	}
	
	if (list_formats_flag) {
		list_formats(url);
		return 0;
	}
	
	// Download the video
	if (download_youtube_video(url, strlen(output_path) > 0 ? output_path : NULL, quality)) {
		printf("\nVideo downloaded successfully!\n");
		return 0;
	} else {
		printf("\nFailed to download video.\n");
		printf("Please check:\n");
		printf("1. Internet connection\n");
		printf("2. Video URL is correct and accessible\n");
		printf("3. You have write permissions in the output directory\n");
		printf("4. The requested quality is available\n");
		return 1;
	}
}
