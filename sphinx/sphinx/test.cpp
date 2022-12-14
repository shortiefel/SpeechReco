/* Start Header**********************************************************************************/
/*
\File					    test.cpp
\Author					    Lee Yu TIng
                            Tan Wei Ling Felicia
\DP email				    yuting.lee@digipen.edu
                            weilingfelicia.tan@digipen.edu
\Date						27/09/2022
\Brief						This file contains the implementation for speech recognition.

Listens continously for mic input and decodes continuously for 100 times. Decoded words 
will be printed to console. Speech recognition will stop running when console is closed. If 
computer has no mic, application will close.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **********************************************************************************/
#include <pocketsphinx.h>
#include <ad.h>
#include <string>
#include <iostream>


#define MODELDIR "model/model_parameters/demo.ci_cont"

ps_decoder_t* pPs;
cmd_ln_t* pConfig;
ad_rec_t* pAd;
int16 adbuf[4096];                                                  // buffer array to hold audio data
uint8 utt_started;                                                  //flag checking if speech has started
uint8 in_speech;                                                    //flag checking if still in speech
int32 numADframes;                                                  // holds the number of frames in the audio buffer

std::string recognize_from_microphone()
{
    ad_start_rec(pAd);                                              // start recording
    ps_start_utt(pPs);                                              // mark the start of the utterance
    utt_started = FALSE;                                            // clear the utt_started flag
    
    while (1) {
        numADframes = ad_read(pAd, adbuf, 4096);                    // capture the number of frames in the audio buffer
        ps_process_raw(pPs, adbuf, numADframes, FALSE, FALSE);      // send the audio buffer to the pocketsphinx decoder

        in_speech = ps_get_in_speech(pPs);                          // test if speech is being detected

        if (in_speech && !utt_started) {                            // if speech has started and utt_started flag is false                            
            utt_started = TRUE;                                     // then set the flag
        }

        if (!in_speech && utt_started) {                            // if speech has ended and utterance had already started
            ps_end_utt(pPs);                                        // end the utterance
            ad_stop_rec(pAd);                                       // stop recording
            const char* hyp = ps_get_hyp(pPs, NULL);                // query pocketsphinx for hypothesis of decoded statement
            if (hyp)
            return hyp;
            return "try again";
        }
    }
}

void Listen()
{
    int numIterations = 0;
    //will decode 100 times before closing
    while (numIterations < 101)
    {
        std::cout << "Listening...\n";
        //capture and decode speech from microphone
        std::string decoded_speech = recognize_from_microphone();
        if (numIterations != 0)
        {
            std::cout << "Decoded Speech: " << decoded_speech << "\n" << std::endl;
        }
        else //first decoding is always inaccurate, need to try again
        {
            std::cout << "Please try again\n";
        }
        ++numIterations;
    }

}
int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    pConfig = cmd_ln_init(NULL, ps_args(), TRUE,
        "-hmm", MODELDIR,
        "-dict", "model/etc/demo.dic",
        "-kws", "model/keywords.list",
        "-remove_noise", "yes",
        //"-remove_silence", "yes",

        NULL);
    if (pConfig == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return -1;
    }

    pPs = ps_init(pConfig);

    if (pPs == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return -1;
    }

    // open default microphone at default samplerate
    pAd = ad_open_dev("sysdefault", (int)cmd_ln_float32_r(pConfig, "-samprate"));
    //if computer has no mic, will exit
    if (pAd == NULL) 
    {
        ps_free(pPs);
        cmd_ln_free_r(pConfig);
        return 0;
    }
    

    Listen();

    //close mic and decoder
    ad_close(pAd);
    ps_free(pPs);
    cmd_ln_free_r(pConfig); 

    return 0;
}