/*
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CSLIST_H_INCLUDED
#define CSLIST_H_INCLUDED

/*
this file is from  my CSimpleLib
github: https://github.com/fuxiang90/CSimpleLib
*/

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CSlistNode_t  CSlistNode;
struct CSlistNode_t {
    struct CSlistNode_t *next;
    void * value;
};
typedef struct CSlist_t  CSlist;
struct CSlist_t{
    struct CSlistNode_t * head;
    struct CSlistNode_t * tail;
    unsigned int len;

};



#define CSlistLength(l) ((l)->len)
#define CSlistFirst(l) ((l)->head)
#define CSlistLast(l) ((l)->tail)



CSlist *CSlistCreate(void);
void CSlistRelease(CSlist * l);
CSlist *CSlistAddNodeHead(CSlist *l, void *value);
CSlist *CSlistAddNodeTail(CSlist *l, void *value);
//CSlist *CSlistInsertNode(CSlist *l, listNode *old_node, void *value, int after);
CSlist *CSlistInsertNode(CSlist *l, CSlistNode *old_node, void *value);
void CSlistDelNode(CSlist *l, CSlistNode *node);
//listIter *CSlistGetIterator(list *list, int direction);
//CSlistNode *listNext(listIter *iter);
//void listReleaseIterator(listIter *iter);
CSlist *listDup(CSlist *orig);
//CSlistNode *CSlistSearchKey(CSlist *l, void *key ,(int*)cmp(void * ,void *) );
CSlistNode *CSlistIndex(CSlist *l, int index);
//void listRewind(CSlist *l, listIter *li);
//void listRewindTail(CSlist *l, listIter *li);

void CSlistPrint(CSlist * l);
#ifdef __cplusplus
}
#endif

#endif // CSLIST_H_INCLUDED
