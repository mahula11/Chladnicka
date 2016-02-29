
enum LOG_TYPE {
	GRAPH,		//* text "GRAPH" to log file
	SD_CARD,	//* empty type in log file
	DEBUG		//* log only to Serial (no to SD card)
};

#define IFCARD(command) \
		if (type == SD_CARD || type == GRAPH) { \
			command; \
																		}
//* String(#x) + String("=") + String(x)
#define LOG_VAR(x) \
		#x << '=' << x

#define LOG_DEBUG1(x) \
		cout << x; 

#define PRINT_STRING(x, y) \
			CLog::getDateTime() << ',' << CLog::getActualFileName(__FILE__).c_str() << ',' << __LINE__ << ',' << y << ',' << x << endl

//* print only on serial
#define LOG_DEBUG(x) \
		{ \
			cout << PRINT_STRING(x, 'D'); \
		}
			//ofstream sdout(CLog::getLogFileName(), ios::app); \
			//sdout << CLog::getDateTime() << ',' << 'D' << ','  << x << endl; \
			//sdout.close(); \
		}

//#define LOG_GRAPH(x) \
//			{ \
//			cout << CLog::getDateTime() << ',' << 'G' << ',' << x << endl; \
//			ofstream sdout(CLog::getLogFileName(), ios::app); \
//			sdout << CLog::getDateTime() << ',' << 'G' << ','  << x << endl; \
//			sdout.close(); \
//			}


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


	//CLog::log(out, NULL, __FILE__, __LINE__ - 5, LOG_TYPE::DEBUG); \

//* print to serial and SD with a GRAPH tag
//#define LOG_GRAPH(...) \
//		{ \
//			File dataFile = SD.open("aa.txt", FILE_WRITE); \
//			CLog::log(&dataFile, __FILE__, __LINE__ - 5, LOG_TYPE::GRAPH, __VA_ARGS__); \
//			dataFile.close(); \
//		}
//
////* print to serial and SD
//#define LOG_SD(...) \
//		{ \
//			File dataFile = SD.open("aa.txt", FILE_WRITE); \
//			CLog::log(&dataFile, __FILE__, __LINE__ - 5, LOG_TYPE::SD_CARD, __VA_ARGS__); \
//			dataFile.close(); \
//		}


//// Addition by NicoHood
//template <typename First, typename... Rest>
//void print(LOG_TYPE type, File & file, const First& first, const Rest&... rest) {
//	printWrapper(0, type, file, first, rest...);
//}
//
//template <typename First>
//void printWrapper(byte comma, LOG_TYPE type, File & file, const First& first) {
//	if (comma == 1) {
//		Serial.print(",");
//		IFCARD(file.print(","));
//		//if (type == SD_CARD || type == GRAPH) {
//		//	file.print(",");
//		//}
//	}
//	comma = 1;
//	Serial.print(first);
//	IFCARD(file.print(first))
//}
//
//template <typename First, typename... Rest>
//void printWrapper(byte comma, LOG_TYPE type, File & file, const First& first, const Rest&... rest) {
//	if (comma == 1) {
//		Serial.print(",");
//		IFCARD(file.print(","))
//	}
//	comma = 1;
//	Serial.print(first);
//	IFCARD(file.print(first))
//	printWrapper(1, type, file, rest...); // recursive call using pack expansion syntax
//}
//
//template <typename First, typename... Rest>
//void println(const First& first, const Rest&... rest) {
//	printlnWrapper(first, rest...);
//}
//
//template <typename First>
//void printlnWrapper(LOG_TYPE type, File & file, const First& first) {
//	//Serial.print("jedno parametrovy println");
//	Serial.print(",");
//	Serial.println(first);
//}
//
//template <typename First, typename... Rest>
//void printlnWrapper(const First& first, const Rest&... rest) {
//	//size_t r = 0;
//	//Serial.print("viac parametrovy println");
//	Serial.print(",");
//	Serial.print(first);
//	printlnWrapper(rest...); // recursive call using pack expansion syntax
//	//return r;
//}

//class CLog {
//public:
//	static DS3231 * _rtc;
//	static String _fileName;
//	static String _date;
//
//	static void setRTC(DS3231 * rtc) {
//		_rtc = rtc;		
//	}
//
//	static void setFileName() {
//		if (_date != _rtc->getDateStr()) {
//			_date = _rtc->getDateStr();
//			Time time = _rtc->getTime();
//			String date = String(time.year - 2000) + 
//							(time.mon < 10 ? "0" : "") + time.mon + 
//							(time.date < 10 ? "0" : "") + time.date; // _rtc->getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, 3);
//			_fileName = 'f' + date + F(".log");
//			//Serial.print(F("file name:"));
//			//Serial.println(_fileName);
//		}
//	}
//
//	template <typename First, typename... Rest>
//	static void log(String file, int line, enum LOG_TYPE type, const First& first, const Rest&... rest) {
//		file.remove(0, file.lastIndexOf('\\') + 1);
//
//		setFileName();
//		
//		nefunguje otvorenie suboru, skusit dat do cpp
//		File dataFile;
//		if (type == GRAPH || type == SD_CARD) {
//			dataFile = SD.open("aa.txt", FILE_WRITE);
//			if (dataFile) {
//				Serial.println("Subor otvoreny");
//			} else {
//				Serial.println("Subor neotvoreny");
//			}
//		}
//		print(type, dataFile, _rtc->getDateStr(), _rtc->getTimeStr(), file, line);
//		printlnWrapper(type, dataFile, first, rest...);
//
//		if (type == GRAPH || type == SD_CARD) {
//			dataFile.close();
//		}
//		
//		//if (type == DEBUG) {
//		//	//* print pnly on Serial (no to file to sd card)
//		//	print(_rtc->getDateStr(), _rtc->getTimeStr(), file, line, 'D');
//		//	printlnWrapper(first, rest...);
//		//} else if (type == SD_CARD) {
//		//	//* print on Serial and to SD card
//		//	print(_rtc->getDateStr(), _rtc->getTimeStr(), file, line, 'S');
//		//	printlnWrapper(first, rest...);
//		//} else if (type == GRAPH) {
//		//	//* print on Serial and SD card with mark "G" as GRAPH
//		//	print(_rtc->getDateStr(), _rtc->getTimeStr(), file, line, 'G');
//		//	printlnWrapper(first, rest...);
//		//}		
//	}
//};



