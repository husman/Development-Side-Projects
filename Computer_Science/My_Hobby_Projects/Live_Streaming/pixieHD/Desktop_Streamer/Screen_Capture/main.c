int main(int argc, char *argv[]) {
	// Start capturing te screen at 1364x768 resolution
	if(ScreenCapture(0, 0, 1364, 768, "testyuv.yuv") == 1) {
		printf("Yuv video saved\n");
	}

	return 0;
}
