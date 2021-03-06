/**
 * @file unicode convert utils
 */

#ifndef __UCONV_H__
#define __UCONV_H__

#ifndef uint16_t              
#define uint16_t unsigned short
#endif //uint16_t             
#ifndef uint8_t
#define uint8_t unsigned char
#endif //uint8_t   

#ifdef __cplusplus
extern "C" {
#endif

#define UCS2_NO_MAPPING (( unsigned short ) 0xfffd)
#define IS_ASCII(a) (0 == (0xff80 & (a)))
/*XXX: 0xa2e3 is euro sign in gbk standard but 0x80 in microsoft's cp936map*/
#define GBK_EURO  ((char)0x80)
#define IS_GBK_EURO(c) ((char)0x80 == (c)) //EURO SIGN
#define UCS2_EURO  (( unsigned short ) 0x20ac)
#define IS_UCS2_EURO(c) ((uint16_t)0x20ac == (c))

#define CAST_CHAR_TO_UNICHAR(a) (( unsigned short )((unsigned char)(a)))
#define CAST_UNICHAR_TO_CHAR(a) ((char)a)

#define UINT8_IN_RANGE(min, ch, max) \
    (((uint8_t)(ch) >= (uint8_t)(min)) && ((uint8_t)(ch) <= (uint8_t)(max)))


#define LEGAL_GBK_MULTIBYTE_FIRST_BYTE(c)  \
          (UINT8_IN_RANGE(0x81, (c), 0xFE))
#define FIRST_BYTE_IS_SURROGATE(c)  \
          (UINT8_IN_RANGE(0x90, (c), 0xFE))
#define LEGAL_GBK_2BYTE_SECOND_BYTE(c) \
          (UINT8_IN_RANGE(0x40, (c), 0x7E)|| UINT8_IN_RANGE(0x80, (c), 0xFE))
#define LEGAL_GBK_4BYTE_SECOND_BYTE(c) \
          (UINT8_IN_RANGE(0x30, (c), 0x39))
#define LEGAL_GBK_4BYTE_THIRD_BYTE(c)  \
          (UINT8_IN_RANGE(0x81, (c), 0xFE))
#define LEGAL_GBK_4BYTE_FORTH_BYTE(c) \
          (UINT8_IN_RANGE(0x30, (c), 0x39))

#define LEGAL_GBK_FIRST_BYTE(c)  \
          (UINT8_IN_RANGE(0x81, (c), 0xFE))
#define LEGAL_GBK_SECOND_BYTE(c) \
          (UINT8_IN_RANGE(0x40, (c), 0x7E)|| UINT8_IN_RANGE(0x80, (c), 0xFE))

enum uconv_flags_t {
	UCONV_INVCHAR_IGNORE = 0x0,  ///< 忽略无效的字符
	UCONV_INVCHAR_REPLACE = 0x1, ///< 使用特殊字符代替无效的字符
	UCONV_INVCHAR_ERROR = 0x2,    ///< 遇到无效字符时返回失败
    UCONV_INVCHAR_ENTITES = 0x4, ///< 把无效的unicode字符转换为html实体字符
    UCONV_INVCHAR_GBK_EURO = 0x8, ///< 把欧元0x80符视为无效GBK.[和之前版本兼容的话,原始EURO为合法GBK]
};

/*
 *  * UTF8 defines and macros
 *   */
#define ONE_OCTET_BASE          0x00    /* 0xxxxxxx */
#define ONE_OCTET_MASK          0x7F    /* x1111111 */
#define CONTINUING_OCTET_BASE   0x80    /* 10xxxxxx */
#define CONTINUING_OCTET_MASK   0x3F    /* 00111111 */
#define TWO_OCTET_BASE          0xC0    /* 110xxxxx */
#define TWO_OCTET_MASK          0x1F    /* 00011111 */
#define THREE_OCTET_BASE        0xE0    /* 1110xxxx */
#define THREE_OCTET_MASK        0x0F    /* 00001111 */
#define FOUR_OCTET_BASE         0xF0    /* 11110xxx */
#define FOUR_OCTET_MASK         0x07    /* 00000111 */
#define FIVE_OCTET_BASE         0xF8    /* 111110xx */
#define FIVE_OCTET_MASK         0x03    /* 00000011 */
#define SIX_OCTET_BASE          0xFC    /* 1111110x */
#define SIX_OCTET_MASK          0x01    /* 00000001 */

#define IS_UTF8_1ST_OF_1(x) (( (x)&~ONE_OCTET_MASK  ) == ONE_OCTET_BASE)
#define IS_UTF8_1ST_OF_2(x) \
((( (x)&~TWO_OCTET_MASK  ) == TWO_OCTET_BASE) && ( (x) != 0xC0 ) && ( (x) != 0xC1))
#define IS_UTF8_1ST_OF_3(x) (( (x)&~THREE_OCTET_MASK) == THREE_OCTET_BASE)
#define IS_UTF8_1ST_OF_4(x) (( (x)&~FOUR_OCTET_MASK ) == FOUR_OCTET_BASE)
#define IS_UTF8_1ST_OF_5(x) (( (x)&~FIVE_OCTET_MASK ) == FIVE_OCTET_BASE)
#define IS_UTF8_1ST_OF_6(x) (( (x)&~SIX_OCTET_MASK  ) == SIX_OCTET_BASE)
#define IS_UTF8_2ND_THRU_6TH(x) \
        (( (x)&~CONTINUING_OCTET_MASK  ) == CONTINUING_OCTET_BASE)
#define IS_UTF8_1ST_OF_UCS2(x) \
        IS_UTF8_1ST_OF_1(x) \
|| IS_UTF8_1ST_OF_2(x) \
|| IS_UTF8_1ST_OF_3(x)

/**
 * @brief 把GBK字符转换为Unicode字符
 *
 * @param a GBK字符的第一个字节
 * @param b GBK字符的第二个字节
 * @return 返回转换后的Unicode字符的值，是一个16位的无符号整数。
 * 如果转换失败，返回无效的Unicode值：0xfffd
 */
// unsigned short  gbk_char_to_unicode(char a, char b);

/**
 * 把 GBK 编码的字符串转换为 Unicode 编码，以16位整数的形式存放。
 *
 * @param src   源字符串，编码为GBK
 * @param src_len    源字符串长度
 * @param dst   目标数组，转化后的 Unicode 码
 * @param dst_size    目标数组的最大长度。
 * @param flags 参考 uconv_flags_t
 * 支持UCONV_INVCHAR_ERROR : 出错时返回-1
 * 支持 UCONV_INVCHAR_REPLACE: 无效字符替换为 UCS2_NO_MAPPING
 * 支持UCONV_INVCHAR_GBK_EURO: GBK_EURO作为无效的GBK字符.默认情况下面是有效的
 * @return 成功返回转化的Unicode字符数。失败返回-1
 * @see uconv_flags_t
 */
int gbk_to_unicode(const char *src, unsigned int src_len,  unsigned short  *dst, unsigned int dst_size,
                   int flags);
    
/**
  * @brief 输入一个Unicode，返回其所对应的GBK字符（两个8位字符）
  *
  * @param unicode 要转化的Unicode字符，
  * @param first_char 指向GBK字符第一个字节的存储位置
  * @param second_char 指向GBK字符第二个字节的存储位置
  *
  * @return 非0表示转换成功，0表示转换失败，即没有相应GBK字符
  */
// unsigned short  unicode_char_to_gbk( unsigned short  unicode, char * first_char, char * second_char);

/**
  * @brief 将Unicode字符转化为GBK字符，最后强制加\0字符，若因此出现半个汉字，则将这个汉字全部删除
  *
  * @param src 要转化的unicode字符串
  * @param src_len 要转化的目标串长度
  * @param dest 转化后的GBK字符串的存储位置
  * @param size 目标串的最大长度
  * @param flags 工作模式，指示当遇到无效Unicode字符时的动作，IGNORE表示忽略，REPLACE表示使用replace_char替换之，E
  *   NTITIES表示使用html实体字符代替
  * 支持UCONV_INVCHAR_GBK_EURO: GBK_EURO作为无效的GBK字符.默认情况下面是有效的
  * @param replace_char 在PEPLACE工作模式下，用于替换无效字符的字符, 0: 表示不替换,1:表示替换.推荐0
  * @return 成功时转换的GBK字符的个数。失败则返回-1
 */
int unicode_to_gbk(const  unsigned short  * src, unsigned int src_len, char * dest, unsigned int size,
                   int flags,  unsigned short  replace_char);

/**
 * 把GBK字符串转化为utf8字符串
 *
 * @param src  gbk字符串
 * @param src_len 原字符串长度
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区最大长度
 * @param flags 工作模式
 * 支持UCONV_INVCHAR_ERROR : 出错时返回-1
 * 支持UCONV_INVCHAR_IGNORE : 忽略出错的字符,在某些情况下，例如gbk文件中包含半个汉字，会出现转换失败的情况（不包含末尾半个汉字的情况，末尾的半个汉字会自动忽略
 * 支持UCONV_INVCHAR_GBK_EURO: GBK_EURO作为无效的GBK字符.默认情况下面是有效的
 * @return 成功返回目标缓冲区长度。失败返回-1
 */

int gbk_to_utf8(const char *src, unsigned int src_len, char *dst, unsigned int dst_size,
                int flags);
	

/**
 * 把UTF8字符串转化为gbk字符串
 *
 * @param src 原utf8字符串
 * @param src_len 原字符串长度
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区大小
 * @param flags 工作模式,支持4种模式
 * 支持UCONV_INVCHAR_GBK_EURO: GBK_EURO作为无效的GBK字符.默认情况下面是有效的
 * @return 成功返回目标缓冲区长度，失败返回-1
 */
int utf8_to_gbk(const char *src, unsigned int src_len, char *dst, unsigned int dst_size,
                int flags);


/**
 * 判断字符串是否是UTF-8编码
 * 注意，本函数不认为全部是英文字符的字符串是UTF-8编码
 * @param str 原字符串 len 原字符串的长度 
 *        check_last_char =1的情况下允许最后一个字符不符合utf8的规则,解决字符串被截断的问题.
 *        该参数是为了解决搜藏中url过长呗浏览器截断的问题，其它项目使用时，推荐设为0.
 * @return 如果是UTF-8返回判断成功的utf8的个数，该数可能小于字符串的长度, 例如在字符串被截断的情况下, 
 * 会把最后的半个utf8去掉.
 * 否则返回0
 */
int is_utf8(const char *str, unsigned int len, int check_last_char);

/**
 * 判断字符串是否是gbk编码, 是则返回1
 */
int uconv_is_gbk(const char * src);

/**
 * 判断字符串是否是gbk编码, 不超过length长度
 * 是则返回1
 */
int uconv_is_gbk_n(const char * src,
                   int length);


#ifndef uint32_t
#define uint32_t unsigned int
#endif
#define UCS4_NO_MAPPING ((uint32_t) 0xfffdffff)
#define CAST_UCS4_TO_UCHAR(a) ((unsigned char)a)
    
/**
 * 把 GB18030 编码的字符串转换为 UCS4 编码，以32位整数的形式存放。
 *
 * @param src   源字符串，编码为GB18030
 * @param src_len    源字符串长度
 * @param dst   目标数组，转化后的 Ucs4 码
 * @param dst_size    目标数组的最大长度。
 * @param flags 参考 uconv_flags_t
 * 支持UCONV_INVCHAR_ERROR : 出错时返回-1
 * 支持 UCONV_INVCHAR_REPLACE: 无效字符替换为 UCS4_NO_MAPPING
 * 支持UCONV_INVCHAR_IGNORE: 忽略转换不成功的字符.
 * 支持UCONV_GBK_EURO: GBK_EURO是有效的GB18030字符.默认情况下面是无效的
 * @return 成功返回转化的Ucs4字符数。失败返回-1
 * @see uconv_flags_t
 */
int gb18030_to_ucs4(const unsigned char *src, 
                    unsigned int src_len,
                    uint32_t *dst,
                    unsigned int dst_size,
                    int flags);

/**
  * @brief 将Ucs4字符转化为GB18030字符，最后强制加\0字符，若因此出现半个汉字，则将这个汉字全部删除
  *
  * @param src 要转化的ucs4字符串
  * @param src_len 要转化的目标串长度
  * @param dest 转化后的GB18030字符串的存储位置
  * @param size 目标串的最大长度
  * @param flags 工作模式，指示当遇到无效Ucs4字符时的动作，
  * IGNORE表示忽略
  * REPLACE表示使用replace_char替换之，
  * ENTITIES表示使用html实体字符代替
  * @param replace_char 在PEPLACE工作模式下，用于替换无效字符的字符.
  *
  * @return 成功时转换的GB18030字符的个数。失败则返回-1
  */
int ucs4_to_gb18030(const uint32_t * src, 
                    unsigned int src_len, 
                    unsigned char * dst, 
                    unsigned int dst_size,
                    int flags,  
                    uint32_t replace_char);
    
/**
 * 把GB18030字符串转化为utf8字符串
 *
 * @param src  gb18030字符串
 * @param src_len 原字符串长度
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区最大长度
 * @param flags 工作模式
 * 支持UCONV_INVCHAR_ERROR : 出错时返回-1
 * 支持UCONV_INVCHAR_IGNORE : 忽略出错的字符,在某些情况下，例如gb18030文件中包含半个汉字，会出现转换失败的情况
 * 支持UCONV_GBK_EURO: GBK_EURO是有效的GB18030字符.默认情况下面是无效的
 * 不包含末尾半个汉字的情况，末尾的半个汉字会自动忽略
 *
 * @return 成功返回目标缓冲区长度。失败返回-1
 */

int gb18030_to_utf8(const unsigned char *src, 
                    unsigned int src_len, 
                    unsigned char *dst, 
                    unsigned int dst_size,
                    int flags);
	

/**
 * 把UTF8字符串转化为gb18030字符串
 *
 * @param src 原utf8字符串
 * @param src_len 原字符串长度
 * @param dst 目标缓冲区
 * @param dst_size 目标缓冲区大小
 * @param flags 工作模式,支持4种模式
 *
 * @return 成功返回目标缓冲区长度，失败返回-1
 */
int utf8_to_gb18030(const unsigned char *src, 
                    unsigned int src_len, 
                    unsigned char *dst, 
                    unsigned int dst_size,
                    int flags);
    
#ifdef __cplusplus
}
#endif
    
#endif // __UCONV_H__
