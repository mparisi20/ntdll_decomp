// GenericTable is a circular doubly linked list
typedef struct GenericTable
{
	u64 unk0;	// zero means table is empty
	Entry *head; // +0x8
	Entry *unk10;
	Entry *recentFound; // +0x18
	u32 recentFoundIndx; // +0x20
	u32 numElements; // +0x24
	u64 unk28;
	u64 unk30;
	u64 unk38;
	u64 unk40;
} GenericTable;

typedef struct ListEntry
{
	ListEntry *next;	// +0x0
	ListEntry *prev;	// +0x8
	void *dataPtr;		// +0x10
}

void RtlInitializeGenericTable
(GenericTable *param1, u64 param2, u64 param3, u64 param4, u64 param5)
{
	param1->unk0 = 0;
	param1->unk10 = &param1->head;
	param1->head = &param1->head;
	param1->recentFound = &param1->head;
	param1->unk40 = param5;
	param1->recentFoundIndx = 0;
	param1->numElements = 0;
	param1->unk28 = param2;
	param1->unk30 = param3;
	param1->unk38 = param4;
}

u32 RtlNumberGenericTableElements(GenericTable *param1)
{
	return param1->numElements;
}

bool RtlIsGenericTableEmpty(GenericTable *param1)
{
	return param1->unk0 == 0;
}

void *RtlGetElementGenericTable(GenericTable *param1, u32 targetIndx)
{
	u32 incTargetIndx = targetIndx+1; // r11d
	u32 recentIndx = param1->recentFoundIndx; // r10d
	unk *found = param1->recentFound; // r8

	if (targetIndx == -1 || incTargetIndx > param1->numElements) {
		return 0;
	}
	
	if (incTargetIndx == recentIndx) {
		return &found->dataPtr;
	}
	if (incTargetIndx < recentIndx) {
		if (incTargetIndx > recentIndx / 2) {
			u32 dist = recentIndx - incTargetIndx;
			while (dist--) {
				found = found->prev;
			}
		} else {
			found = &param1->head;
			while (incTargetIndx--) {
				found = found->next;
			}
		}
	} else {
		u32 distFromRecent = incTargetIndx - recentIndx;
		u32 distFromHead = param1->numElements - incTargetIndx + 1;
		if (distFromRecent <= distFromHead) {
			while (distFromRecent--) {
				found = found->next; // move forwards
			}
		} else {
			found = &param1->head;
			while (distFromHead--) {
				found = found->prev; // move backwards
			}
		}
	}	
	param1->recentFound = found;
	param1->recentFoundIndx = targetIndx+1;
	return &found->dataPtr;
}