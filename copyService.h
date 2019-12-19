
/*
Creates a copy job for source file to destination file 
returns the job id
*/
int copy_createjob(const char* src, const char* dst);

/*
Canceles a copy job based on the id
*/
void copy_cancel(int jobID);

/*
Pause a copy job based on the id
*/
void copy_pause(int jobID);
/*
Returns a copy job status based on the id
*/
char* copy_status(int jobID);

/*
Returns the progress of a copy job(0-100)
*/
int copy_progress(int jobID);

/*
Returns a list of job status
*/
int copy_listjobs(char** statusList, int* jobsCount);

/*
Returns status of all the copy jobs
*/
char* copy_freestats();
