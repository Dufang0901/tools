#ifndef ADPCM_H
#define ADPCM_H
#ifdef __cplusplus
extern "C" {
#endif
struct adpcm_state {
    short valprev; /* Previous output value */
    char index;  /* Index into stepsize table */
};
void adpcm_decoder(char *inbuff,char *outbuff,int len_of_in,struct adpcm_state * state );
#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* ADPCM_H*/