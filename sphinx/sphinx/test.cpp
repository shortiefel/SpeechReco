#include <pocketsphinx.h>
#include <ad.h>
#include <string>
#include <iostream>
#include <thread>
#include "windows.h"
#include <future>
#include <mutex>

//#define MODELDIR "C:/Users/65979/Documents/Sphinx/pocketsphinx/model"
//#define MODELDIR "C:/cmuSphinx/demoTrain/model_parameters/demo.ci_cont"
#define MODELDIR "Sphinx/pocketsphinx/model/model_parameters/demo.ci_cout"

ps_decoder_t* ps;
cmd_ln_t* config;
FILE* fh;
char const* hyp, * uttid;
int16 buf[512];
int rv;
int32 score;
ad_rec_t* ad;
int16 adbuf[4096];                 // buffer array to hold audio data
uint8 utt_started, in_speech;      // flags for tracking active speech - has speech started ? -is speech currently happening ?
int32 k;                           // holds the number of frames in the audio buffer
std::string recognize_from_microphone()
{
    ad_start_rec(ad);                                // start recording
    ps_start_utt(ps);                                // mark the start of the utterance
    utt_started = FALSE;                             // clear the utt_started flag
    
    while (1) {
        k = ad_read(ad, adbuf, 4096);                // capture the number of frames in the audio buffer
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);  // send the audio buffer to the pocketsphinx decoder

        in_speech = ps_get_in_speech(ps);            // test to see if speech is being detected

        if (in_speech && !utt_started) {             // if speech has started and utt_started flag is false                            
            utt_started = TRUE;                      // then set the flag
        }

        if (!in_speech && utt_started) {             // if speech has ended and the utt_started flag is true 
            ps_end_utt(ps);                          // then mark the end of the utterance
            ad_stop_rec(ad);                         // stop recording
            hyp = ps_get_hyp(ps, NULL);             // query pocketsphinx for "hypothesis" of decoded statement
            if (hyp)
            return hyp;// the function returns the hypothesis
            return "";
            break;                                   // exit the while loop and return to main
        }
    }
}

void Listen()
{
    while (1)
    {
        std::cout << "Listening\n";
        std::string decoded_speech = recognize_from_microphone();          // call the function to captureand decode speech
        std::cout << "Decoded Speech: " << decoded_speech << "\n" << std::endl;   // send decoded speech to screen
    }

}
int main(int argc, char* argv[])
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //For memory check
#endif
    //config = cmd_ln_init(NULL, ps_args(), TRUE,
    //    "-hmm", MODELDIR "/en-us/en-us",
    //    "-lm", MODELDIR "/en-us/en-us.lm.bin",
    //    "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
    //    NULL);

    config = cmd_ln_init(NULL, ps_args(), TRUE,
        "-hmm", MODELDIR,
        //"-lm", "C:/cmuSphinx/demoTrain/etc/demo.lm",
        "-dict", "C:/cmuSphinx/demoTrain/etc/demo.dic",
        "-kws", "C:/cmuSphinx/sphinx/sphinx/keywords.list",
        "-remove_noise", "yes",
        NULL);
    if (config == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return -1;
    }

    ps = ps_init(config);
    //keyword??? 4/9
    //const char* KWS_SEARCH = "keywords";
    //ps_set_kws(ps, KWS_SEARCH, "C:/cmuSphinx/sphinx/sphinx/keywords.list");
    //ps_set_keyphrase(ps, )
    //ps_set_search(ps, KWS_SEARCH);
    if (ps == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return -1;
    }

    ad = ad_open_dev("sysdefault", (int)cmd_ln_float32_r(config, "-samprate"));
    // open default microphone at default samplerate
    

    //std::async(std::launch::deferred, Listen);
    std::thread t2(Listen);

    //while (1) {

    //    std::string decoded_speech = recognize_from_microphone();          // call the function to captureand decode speech
    //        std::cout << "Decoded Speech: " << decoded_speech << "\n" << std::endl;   // send decoded speech to screen
    //}

    t2.join();
    //while (1)
    //{
    //    //Sleep(50);
    //    std::cout << "do other stuff\n";
    //}
    ad_close(ad); //close mic
    ps_free(ps);
    cmd_ln_free_r(config);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////recognizing from raw files:
    /*fh = fopen("goforward.raw", "rb");
    if (fh == NULL) {
        fprintf(stderr, "Unable to open input file goforward.raw\n");
        return -1;
    }

    rv = ps_start_utt(ps);

    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh); //read 512 bytes a time
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE); //feeding utterance to decoder
    }

    rv = ps_end_utt(ps); //mark end of utterance
    hyp = ps_get_hyp(ps, &score);
    printf("Recognized: %s\n", hyp);

    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);*/

    return 0;
}