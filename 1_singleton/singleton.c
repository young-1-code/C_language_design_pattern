#include "singleton.h"

/**
 * @brief: 以追加方式创建一个日志文件
 * @log: 传入一个log对象
 * @return: 0:ok -1:err
*/
static int _open_file(log_t *log)
{
    if(!log) return -1;
    time_t rawtime;
    struct tm *tminfo=NULL;

    time(&rawtime);
    tminfo = localtime(&rawtime);

    snprintf(log->log_name, 128, "%s_T%d-%d-%d_%d:%d:%d.log", 
        LOG_FILE_NAME,  tminfo->tm_year+1900, tminfo->tm_mon+1, tminfo->tm_mday, \
        tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec);
    log->wfile = fopen(log->log_name, "a");

    return log->wfile==NULL ? -1 : 0;
}

/**
 * @brief: 将日志内容写入文件
 * @buffer: 日志内容
 * @return: 0:ok -1:err
*/
static int _write_file(const char* buffer)
{
    static unsigned char is_check=1;
    struct stat log_fsta;
    size_t ret=0;
    
    if(!buffer || !singleton_log) return -1;
    #ifdef _REENTRANT //是否使用多线程
    pthread_mutex_lock(&log_mutex);
    #endif

    ret = fwrite(buffer, 1, strlen(buffer), singleton_log->wfile);

    // if( (++is_check) % 16 != 0 ) goto exit;       //写入16次检测一次是否要将日志写入另外一个文件
    if( ((++is_check)&0xF) != 0 ) goto exit;         //写入16次检测一次是否要将日志写入另外一个文件
    
    fflush(singleton_log->wfile);
    stat( singleton_log->log_name, &log_fsta );      //获取文件的大小

    if( log_fsta.st_size > singleton_log->log_size ){ //写到设定值，重新打开一个文件写入
        fclose( singleton_log->wfile );               //关闭当前文件
        _open_file(singleton_log);                    //创建一个新文件
    }

exit:
    #ifdef _REENTRANT //是否使用多线程
    pthread_mutex_unlock(&log_mutex);
    #endif
    return ret;
}

/**
 * @brief: log记录debug的日志信息
 * @input: 传入可变参数
 * @note: 采用static修饰，外部其他文件不能直接调用，
 *  赋值给函数指针，通过函数指针进行调用
 * @return: 0:ok -1:err
*/
static int log_debug(const char *__restrict __fmt, ...)
{
    va_list args;
    time_t rawtime;
    struct tm *tminfo=NULL;
    int size=0, ret = 0;
    char buffer[1024], time_buf[128];

    if(!singleton_log) return -1;

    time(&rawtime);
    tminfo = localtime(&rawtime);

    // strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tminfo);
    size = sprintf(buffer, "[debug]: Time:%d-%d-%d %d:%d:%d File:%s Line:%d @: ", 
                    tminfo->tm_year+1900, tminfo->tm_mon+1, tminfo->tm_mday, \
                    tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec, __FILE__, __LINE__);

    va_start(args, __fmt);                                 //定义可变参数列表
    size = vsnprintf(buffer+size, 1024-size, __fmt, args); //buffer+size是偏移前面固定信息，避免被覆盖
    va_end(args);

    return _write_file(buffer);
}

/**
 * @brief: log记录警告的日志信息
 * @input: 传入可变参数
 * @note: 采用static修饰，外部其他文件不能直接调用，
 *  赋值给函数指针，通过函数指针进行调用
 * @return: 0:ok -1:err
*/
static int log_warning(const char *__restrict __fmt, ...)
{
    va_list args;
    time_t rawtime;
    struct tm *tminfo=NULL;
    int size=0, ret = 0;
    char buffer[1024], time_buf[128];

    if(!singleton_log) return -1;

    time(&rawtime);
    tminfo = localtime(&rawtime);

    size = sprintf(buffer, "[warning]: Time:%d-%d-%d %d:%d:%d File:%s Line:%d @: ", 
                    tminfo->tm_year+1900, tminfo->tm_mon+1, tminfo->tm_mday, \
                    tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec, __FILE__, __LINE__);

    va_start(args, __fmt);
    size = vsnprintf(buffer+size, 1024-size, __fmt, args);
    va_end(args);
    
    return _write_file(buffer);
}

/**
 * @brief: log记录错误的日志信息
 * @input: 传入可变参数
 * @note: 采用static修饰，外部其他文件不能直接调用，
 *  赋值给函数指针，通过函数指针进行调用
 * @return: 0:ok -1:err
*/
static int log_error(const char *__restrict __fmt, ...)
{
    va_list args;
    time_t rawtime;
    struct tm *tminfo=NULL;
    int size=0, ret = 0;
    char buffer[1024], time_buf[128];

    if(!singleton_log) return -1;

    time(&rawtime);
    tminfo = localtime(&rawtime);

    size = sprintf(buffer, "[error]: Time:%d-%d-%d %d:%d:%d File:%s Line:%d @: ", 
                    tminfo->tm_year+1900, tminfo->tm_mon+1, tminfo->tm_mday, \
                    tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec, __FILE__, __LINE__);

    va_start(args, __fmt);
    size = vsnprintf(buffer+size, 1024-size, __fmt, args);
    va_end(args);

    return _write_file(buffer);
}

/**
 * @brief: 释放log内存
 * @note: 采用static修饰，外部其他文件不能直接调用，
 *  赋值给函数指针，通过函数指针进行调用
*/
static void log_destroy()
{
    if(!singleton_log) return;

    if(singleton_log->wfile) fclose( singleton_log->wfile );
    singleton_log->wfile = NULL;
    
    if(singleton_log) free(singleton_log);
    singleton_log = NULL;
}

/**
 * @brief: 构造函数，创建一个log对象
 * @size: 日志文件最大值
 * @filename: 日志文件名字
 * @note: 采用static 修饰，外部其他文件不能直接调用来创建对象，
 *  保证系统中存在唯一的单例。
 * @return: log:ok NULL:err
*/
static log_t* construct_singleton_log(int size, const char* filename)
{
    static int inst_times=0;            //设置一个变量，确保只创建唯一log对象

    if(inst_times!=0 || !filename || size<=0) return NULL;
    
    log_t* obj = (log_t*)malloc(sizeof(log_t));
    if(!obj) return NULL;
    memset(obj, 0, sizeof(obj));

    //创建日志文件
    if(_open_file(obj)==-1) return NULL;
    obj->log_size = size;

    obj->debug = log_debug;             //给指针函数赋值
    obj->warning = log_warning;
    obj->error = log_error;
    obj->destroy = log_destroy;
    inst_times++;

    return obj;
}

/**
 * @brief: 懒汉单例模式, 供外部调用访问
 * @return: 返回一个单例对象
*/
log_t* log_get_inst (void)
{
    if(!singleton_log){  //双检锁
        #ifdef _REENTRANT //是否使用多线程
        pthread_mutex_lock(&log_mutex);       
        #endif
        if(!singleton_log){
            singleton_log = construct_singleton_log(LOG_FILE_SIZE, LOG_FILE_NAME);
        } 
        #ifdef _REENTRANT //是否使用多线程
        pthread_mutex_unlock(&log_mutex); 
        #endif
    }
    return singleton_log;
}
