#ifndef __UTIL_MPM_B2G_H__
#define __UTIL_MPM_B2G_H__

#include "util-mpm.h"
#include "util-bloomfilter.h"

#define B2G_NOCASE 0x01
#define B2G_SCAN   0x02

//#define B2G_HASHSIZE 65536
//#define B2G_HASHSIZE 32768
//#define B2G_HASHSIZE 16384
//#define B2G_HASHSIZE 8192
#define B2G_HASHSIZE 4096
//#define B2G_HASHSIZE 2048

//#define B2G_HASHSHIFT 8
//#define B2G_HASHSHIFT 7
//#define B2G_HASHSHIFT 6
//#define B2G_HASHSHIFT 5
#define B2G_HASHSHIFT 4
//#define B2G_HASHSHIFT 3

//#define B2G_TYPE uint64_t
#define B2G_TYPE uint32_t
//#define B2G_TYPE uint16_t
//#define B2G_TYPE uint8_t
//#define B2G_WORD_SIZE 64
#define B2G_WORD_SIZE 32
//#define B2G_WORD_SIZE 16
//#define B2G_WORD_SIZE 8

#define B2G_BLOOMSIZE 1024

#define B2G_HASH16(a,b) (((a)<<B2G_HASHSHIFT) | (b))
#define B2G_Q           2

#define B2G_SCANFUNC B2gScanBNDMq
//#define B2G_SCANFUNC B2gScan

#define B2G_SEARCHFUNC B2gSearchBNDMq
//#define B2G_SEARCHFUNC B2gSearch

//#define B2G_SCAN2
//#define B2G_COUNTERS

typedef struct B2gPattern_ {
    uint8_t flags;
    uint16_t len;
    uint8_t *cs; /* case sensitive */
    uint8_t *ci; /* case INsensitive */
    struct B2gPattern_ *next;
    MpmEndMatch *em;
} B2gPattern;

typedef struct B2gHashItem_ {
    uint16_t idx;
    struct B2gHashItem_ *nxt;
    uint8_t flags;
} B2gHashItem;

typedef struct B2gCtx_ {
    B2G_TYPE *scan_B2G;
    B2G_TYPE scan_m;
    BloomFilter **scan_bloom;
    uint8_t *scan_pminlen; /* array containing the minimal length
                               of the patters in a hash bucket. Used
                               for the BloomFilter. */
    /* pattern arrays */
    B2gPattern **parray;

    B2G_TYPE search_m;
    B2G_TYPE *search_B2G;

    uint16_t scan_1_pat_cnt;
#ifdef B2G_SCAN2
    uint16_t scan_2_pat_cnt;
#endif
    uint16_t scan_x_pat_cnt;

    uint32_t scan_hash_size;
    B2gHashItem **scan_hash;
    B2gHashItem scan_hash1[256];
#ifdef B2G_SCAN2
    B2gHashItem **scan_hash2;
#endif
    uint32_t search_hash_size;
    BloomFilter **search_bloom;
    uint8_t *search_pminlen; /* array containing the minimal length
                                 of the patters in a hash bucket. Used
                                 for the BloomFilter. */
    B2gHashItem **search_hash;
    B2gHashItem search_hash1[256];

    /* hash used during ctx initialization */
    B2gPattern **init_hash;

    uint8_t scan_s0;
    uint8_t search_s0;

    /* we store our own multi byte scan ptr here for B2gSearch1 */
    uint32_t (*Scan)(struct MpmCtx_ *, struct MpmThreadCtx_ *, PatternMatcherQueue *, uint8_t *, uint16_t);
    /* we store our own multi byte search ptr here for B2gSearch1 */
    uint32_t (*Search)(struct MpmCtx_ *, struct MpmThreadCtx_ *, PatternMatcherQueue *, uint8_t *, uint16_t);

    /* we store our own multi byte scan ptr here for B2gSearch1 */
    uint32_t (*MBScan2)(struct MpmCtx_ *, struct MpmThreadCtx_ *, PatternMatcherQueue *, uint8_t *, uint16_t);
    uint32_t (*MBScan)(struct MpmCtx_ *, struct MpmThreadCtx_ *, PatternMatcherQueue *, uint8_t *, uint16_t);
    /* we store our own multi byte search ptr here for B2gSearch1 */
    uint32_t (*MBSearch)(struct MpmCtx_ *, struct MpmThreadCtx_ *, PatternMatcherQueue *, uint8_t *, uint16_t);

} B2gCtx;

typedef struct B2gThreadCtx_ {
#ifdef B2G_COUNTERS
    uint32_t scan_stat_pminlen_calls;
    uint32_t scan_stat_pminlen_total;
    uint32_t scan_stat_bloom_calls;
    uint32_t scan_stat_bloom_hits;
    uint32_t scan_stat_calls;
    uint32_t scan_stat_m_total;
    uint32_t scan_stat_d0;
    uint32_t scan_stat_d0_hashloop;
    uint32_t scan_stat_loop_match;
    uint32_t scan_stat_loop_no_match;
    uint32_t scan_stat_num_shift;
    uint32_t scan_stat_total_shift;

    uint32_t search_stat_d0;
    uint32_t search_stat_loop_match;
    uint32_t search_stat_loop_no_match;
    uint32_t search_stat_num_shift;
    uint32_t search_stat_total_shift;
#endif /* B2G_COUNTERS */
} B2gThreadCtx;

void MpmB2gRegister(void);


#endif

