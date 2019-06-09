
#include <string.h> // memset()

#include "ipps.h"
#include "ippsc.h"
#include "g729fpapi.h"

#include "g729a_ipp_interface.h"

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

typedef struct{
	G729FPDecoder_Obj *psDecObj;
    void *psDecBuf;
}G729A_IPP_DECODER;

typedef struct{
    G729FPEncoder_Obj *psEncObj;
    void *psEncBuf;
}G729A_IPP_ENCODER;

const int FRAMESIZE = 80;
const int G729_SERIALSIZE = 10;

int G729A_Get_Decoder_Size(int *decoder_bytes)
{
	int ret = 0;

	*decoder_bytes = sizeof( G729A_IPP_DECODER );

	return ret;
}

int createG729FPDecoder(G729A_IPP_DECODER *decoder)
{
	if (!decoder) 
		return 1;

	int ret = 0;
	memset(decoder, 0, sizeof(decoder));

    do {
		void * ptr;

		int scratchMemSize = 0;
		apiG729FPCodec_ScratchMemoryAlloc(&scratchMemSize);

		// Create decoder and Initialize buffer and encoder
		int decoder_bytes = 0;
		apiG729FPDecoder_Alloc( (G729Codec_Type)1, (int *)&decoder_bytes );
		ptr = malloc( decoder_bytes );
		if ( !ptr ) {
			ret = __LINE__;
            break;
		}
		decoder->psDecObj = (G729FPDecoder_Obj *)ptr;


		ptr = malloc( scratchMemSize );
		if ( !ptr ) {
			ret = __LINE__;
            break;
		}
		decoder->psDecBuf = ptr;

		apiG729FPDecoder_InitBuff(decoder->psDecObj, (char *)ptr);
		apiG729FPDecoder_Init(decoder->psDecObj, (G729Codec_Type)1);
    }while(0);

 	if ( !ret ) {
        return 0;
    }

    if ( decoder->psDecObj )
        free( decoder->psDecObj );

    // Why set the memory 0 after initialize the decoder?
    memset(decoder, 0, sizeof(G729A_IPP_DECODER));

    return ret;
}

int G729A_InitDecoder(void* codec)
{
    int ret = 0;
    G729A_IPP_DECODER* decoder;

    decoder = (G729A_IPP_DECODER *)codec;

    ret = createG729FPDecoder( decoder );
    if ( ret ) {
         printf("Create G729 decoder failed\n");
         return 1;
    }

    return ret;
}

void G729A_DestoryDecoder(void* codec)
{
	if(!codec)
		return ;
	
	G729A_IPP_DECODER* decoder;
	decoder = (G729A_IPP_DECODER *)codec;

	SAFE_FREE(decoder->psDecBuf);
	SAFE_FREE(decoder->psDecObj);

	memset(decoder, 0, sizeof(G729A_IPP_DECODER));
	// What's the function of this method?
	//Attr_API(606536, 1);
}

int G729A_IPP_Decode(void *psCodec, unsigned char *g729_data, int g729_len, unsigned short *pcm_data, int *pcm_samples)
{
	G729A_IPP_DECODER* decoder;

	decoder = (G729A_IPP_DECODER*)psCodec;
//  Attr_API(594243, 1);
    int frames = g729_len / G729_SERIALSIZE;

    if (g729_len != G729_SERIALSIZE * frames) {
    	/*
		ERROR("Unsupported g729 len %d, frame %d", g729_len, frames);
		LOG_ERROR_WITH("Unsupported g729 len %d, frame %d", g729_len, frames);
		*/
		*pcm_samples = 0;
		return 1;
	}

	int i, ret = 0;
	for (i = 0; i < frames && !ret; i++) {
		int frametype = 3;
		ret = apiG729FPDecode(decoder->psDecObj,
				(const Ipp8u*)(g729_data + i * G729_SERIALSIZE),
				frametype,
				(short *)(pcm_data + 80 * i));
	}

	if (ret) {
		*pcm_samples = 0;
		ret = 1;
	} else {
		*pcm_samples = FRAMESIZE * frames;
		ret = 0;
	}

	return ret;
}

int G729A_Get_Encoder_Size(int *encoder_bytes)
{
	int ret = 0;

	*encoder_bytes = sizeof( G729A_IPP_ENCODER );

	return ret;
}

int createG729FPEncoder(G729A_IPP_ENCODER *encoder)
{
    if (!encoder)
        return 1;

    int ret = 0;
    memset(encoder, 0, sizeof(encoder));

    do {
		void * ptr;

		int scratchMemSize = 0;
		apiG729FPCodec_ScratchMemoryAlloc(&scratchMemSize);

		// Create encoder and Initialize buffer and encoder
		int encoder_bytes = 0;
		apiG729FPEncoder_Alloc((G729Codec_Type)1, (int *)&encoder_bytes);

		ptr = malloc(encoder_bytes);
		if (!ptr) {
			ret = __LINE__;
            break;
		}

		encoder->psEncObj = (G729FPEncoder_Obj *)ptr;

		ptr = malloc(scratchMemSize);
		if ( !ptr ) {
			ret = __LINE__;
            break;
		}
		encoder->psEncBuf = ptr;

		apiG729FPEncoder_InitBuff(encoder->psEncObj, (char *)ptr);
		apiG729FPEncoder_Init(encoder->psEncObj, (G729Codec_Type)1, (G729Encode_Mode)0);

    }while(0);

    if ( !ret ) {
        return 0;
    }

    if ( encoder->psEncObj )
        free( encoder->psEncObj );

    memset(encoder, 0, sizeof(G729A_IPP_ENCODER));

    return ret;
}

int G729A_InitEncoder(void* codec)
{
    int ret = 0;
    G729A_IPP_ENCODER* encoder;

    encoder = (G729A_IPP_ENCODER *)codec;

    ret = createG729FPEncoder( encoder );
    if ( ret ) {
         printf("Create G729 encoder failed\n");
         return 1;
    }

    return ret;
}

void G729A_DestoryEncoder(void* codec)
{
	if(!codec)
		return ;
	
	G729A_IPP_ENCODER* encoder;
	encoder = (G729A_IPP_ENCODER *)codec;

	SAFE_FREE(encoder->psEncBuf);
	SAFE_FREE(encoder->psEncObj);

	memset(encoder, 0, sizeof(G729A_IPP_ENCODER));
	//Attr_API(606536, 1);
}



int G729A_IPP_Encode(void *psCodec, unsigned short *pcm_data, int pcm_samples, unsigned char *g729_data, int *g729_len)
{
	G729A_IPP_ENCODER* encoder;
	encoder = (G729A_IPP_ENCODER*)psCodec;
//  Attr_API(594244, 1);
    int i, ret = 0;
    int frame = pcm_samples / 80;
    for (i = 0; i < frame && !ret; i++) {
        int frametype = 0;
        ret = apiG729FPEncode(encoder->psEncObj,
				(const short*)(pcm_data + 80 * i),
				(Ipp8u*)(g729_data + G729_SERIALSIZE * i),
				(G729Codec_Type)1,
				&frametype);
    }

    if (ret) {
        *g729_len = 0;
		ret = 1;
    } else {
		*g729_len = G729_SERIALSIZE * frame;
		ret = 0;
	}

	return ret;
}
