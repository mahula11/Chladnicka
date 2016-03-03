
//enum LOG_TYPE {
//	GRAPH,		//* text "GRAPH" to log file
//	SD_CARD,	//* empty type in log file
//	DEBUG		//* log only to Serial (no to SD card)
//};
																	
//* String(#x) + String("=") + String(x)
#define LOG_VAR(x) \
		#x << '=' << x

#define PRINT_STRING(x, y) \
			CLog::getDateTime() << ',' << CLog::getActualFileName(__FILE__) << ',' << __LINE__ << ',' << y << ',' << x << endl

//* print only on serial
#define LOG_DEBUG(x) \
		{ \
			cout << PRINT_STRING(x, 'D'); \
		}

#define LOG_GRAPH(x) \
		{ \
			cout << PRINT_STRING(x, 'G'); \
			ofstream sdout(CLog::getLogFileName(), ios::app); \
			sdout << PRINT_STRING(x, 'G'); \
			sdout.close(); \
		}

#define LOG_SD(x) \
		{ \
			cout << PRINT_STRING(x, 'S'); \
			ofstream sdout(CLog::getLogFileName(), ios::app); \
			sdout << PRINT_STRING(x, 'S'); \
			sdout.close(); \
		}

