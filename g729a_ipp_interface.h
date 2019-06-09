
#ifndef __G729IPP_INTERFACE_H__
#define __G729IPP_INTERFACE_H__


#ifdef __cplusplus
extern "C" {
#endif

int G729A_Get_Decoder_Size(int *decoder_bytes);

int G729A_InitDecoder(void* codec);

void G729A_DestoryDecoder(void* codec);

int G729A_Get_Encoder_Size(int *encoder_bytes);

int G729A_InitEncoder(void* codec);

void G729A_DestoryEncoder(void* codec);

/**********************************************************************
 * G729IPP_Encode(...)
 */

int G729A_IPP_Encode(void *psCodec, 
					 unsigned short *pcm_data, 
					 int pcm_samples, 
					 unsigned char *g729_data, 
					 int *g729_len); 

/**********************************************************************
 * G729IPP_Decode(...)
 */
int G729A_IPP_Decode(void *psCodec,
					 unsigned char *g729_data, 
					 int g729_len, 
					 unsigned short *pcm_data, 
					 int *pcm_samples);


#ifdef __cplusplus
}
#endif

#endif // __G729IPP_INTERFACE_H__
