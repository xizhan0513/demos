#ifndef __DEBUG_H__
#define __DEBUG_H__

/* log level */
enum {
	_DEMO_NONE_    = 0,
	_DEMO_ALWAYS_  = 1,
	_DEMO_ERR_     = 2,
	_DEMO_WARNING_ = 3,
	_DEMO_INFO_    = 4,
	_DEMO_DEBUG_   = 5,
	_DEMO_MAX_     = 6,
};

#define DEMO_PREFIX "DEMO: "
#define _dbgdump printf

#undef  DEMO_ALWAYS
#define DEMO_ALWAYS(fmt, arg...)	\
	do { \
		if (_DEMO_ALWAYS_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)

#undef  DEMO_ERR
#define DEMO_ERR(fmt, arg...)	\
	do { \
		if (_DEMO_ERR_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)

#undef  DEMO_WARNING
#define DEMO_WARNING(fmt, arg...)	\
	do { \
		if (_DEMO_WARNING_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)

#undef  DEMO_INFO
#define DEMO_INFO(fmt, arg...)	\
	do { \
		if (_DEMO_INFO_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)

#undef  DEMO_DEBUG
#define DEMO_DEBUG(fmt, arg...)	\
	do { \
		if (_DEMO_DEBUG_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)


#undef  DEMO_MAX
#define DEMO_MAX(fmt, arg...)	\
	do { \
		if (_DEMO_MAX_ <= log_level) { \
			_dbgdump(DEMO_PREFIX fmt, ##arg); \
		} \
	} while (0)

#endif
