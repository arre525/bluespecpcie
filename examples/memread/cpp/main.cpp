#include <stdio.h>
#include <unistd.h>

#include "bdbmpcie.h"
#include "dmasplitter.h"


int main(int argc, char** argv) {
	BdbmPcie* pcie = BdbmPcie::getInstance();
	DMASplitter* dma = DMASplitter::getInstance();

	uint32_t size = 128*128;

	if ( argc > 1 ) {
		size = atoi(argv[1]);
	}

	unsigned int d = pcie->readWord(0);
	printf( "Magic: %x\n", d );
	fflush(stdout);


	uint8_t* dmabuf = (uint8_t*)dma->dmaBuffer();
	for ( int i = 0; i < 8*1024; i++ ) {
		dmabuf[i] = (char)i;
	}
	for ( int i = 0; i < 16; i++ ) {
		dmabuf[i] = 0xaa;
	}
	printf( "Requesting memread with size %d\n", size );
	dma->sendWord(0,size,0,0,0);

	while (1) {
		PCIeWord w = dma->recvWord();
		printf( "%x %x %x %x %x\n", w.header, w.d[0], w.d[1], w.d[2], w.d[3] );
		if ( w.header == 0 ) break;
	}





/*
	FlashManager* flash = FlashManager::getInstance();

	uint32_t* pageBufferW = (uint32_t*)malloc(8192+32);
	uint32_t* pageBufferR = (uint32_t*)malloc(8192+32);
	for ( int i = 0; i < 8192/4; i++ ) {
		pageBufferW[i] = i;
	}
	printf( "Sending erase message:\n" );
	flash->eraseBlock(1,1,1);
	//sleep(1);
	flash->writePage(1,1,1,0, pageBufferW);

	sleep(5);

	exit(1);

	
	printf( "\t\tSending read cmd\n" );


	flash->readPage(1,1,1,0, pageBufferR);
	
	
	printf( "\t\tStarting read\n" );
	sleep(1);
	
	for ( int i = 0; i < (8192+32)/4; i++ ) {
		int idx = i % 4;
		uint32_t r = pcie->readWord((1024+1+idx)*4);
		printf( "%d:%x ", i, r );
		if ( i % 8 == 0 ) printf( "\n" );
	}
	*/



/*
	for ( int i = 0; i < 1024; i++ ) {
		pcie->writeWord((1024+3)*4, 0);
	}
	
	pcie->writeWord((1024+1)*4, 0);

	pcie->writeWord(1024*4, 4);
	for ( int i = 0; i < 32; i++ ) {
		pcie->writeWord(1024*4, 8);
	}
	sleep(1);
	pcie->writeWord((1024+1)*4, 0);


	for ( int i = 0; i < 4096; i++ ) {
		if ( ubuf[i] != 0 ) {
			printf( "%d: %x\n", i, ubuf[i] );
		}
	}
	*/
}
