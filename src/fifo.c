#include "bootpack.h"
#define FLAGS_OVERRUN 		0x0001
#define FLAGS_RESI 			0x0000
void queue8_init(struct Queue8 *queue, int size, unsigned char *buf)
{
	/* 初始化queue，类似于构造函数 */
	queue->size = size;
	queue->freeSpace = size;
	queue->toReadLoc = 0;
	queue->toWriteLoc = 0;
	queue->flag = FLAGS_RESI;
	queue->buf = buf;
	queue->empty = TRUE;
}

int queue8_push(struct Queue8 *queue, unsigned char data)
{
	if(queue->freeSpace <= 0)
	{
		queue->flag = FLAGS_OVERRUN;
		return FALSE;
	}

	if(queue->toWriteLoc >= queue->size)
	{
		/* 写入指针超出边界时把数据整体往前移动 */
		int i;
		for (i = queue->toReadLoc; i < queue->size; ++i)
		{
			/* code */
			queue->buf[i - queue->toReadLoc] = queue->buf[i];
		}
		queue->toReadLoc = 0;
		queue->toWriteLoc = queue->toWriteLoc - queue->toReadLoc;
	}

	queue->buf[queue->toWriteLoc] = data;
	queue->toWriteLoc++;
	queue->freeSpace--;
	return TRUE;
}

int queue8_pop(struct Queue8 *queue)
{
	return TRUE;
}

int queue8_status(struct Queue8 *queue)
{
	return TRUE;
}