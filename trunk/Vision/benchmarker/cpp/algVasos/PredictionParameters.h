//File to define prediction system parameters


//distance in px 
#define PREDICTION_DELTA 20
//number of frames for a garbage to be considered by the prediction system
#define PREDICTION_MIN_AGE 3
//default number of frames that a garbage cannot be recognized until prediction system erase it
#define PREDICTION_DEFAULT_FRAMES_NOAPPEAR 5
//number of frames for focusing system to select a garbage
#define NUMBER_OF_FRAMES_TO_FOCUS 20
//period to refresh maxFramesNoAppear
#define PREDICTION_REFRESH_AGE 10
