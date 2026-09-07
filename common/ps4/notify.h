#pragma once

enum NotifyType
{
	NotificationRequest = 0,
	SystemNotification = 1,
	SystemNotificationWithUserId = 2,
	SystemNotificationWithDeviceId = 3,
	SystemNotificationWithDeviceIdRelatedToUser = 4,
	SystemNotificationWithText = 5,
	SystemNotificationWithTextRelatedToUser = 6,
	SystemNotificationWithErrorCode = 7,
	SystemNotificationWithAppId = 8,
	SystemNotificationWithAppName = 9,
	SystemNotificationWithAppInfo = 9,
	SystemNotificationWithAppNameRelatedToUser = 10,
	SystemNotificationWithParams = 11,
	SendSystemNotificationWithUserName = 12,
	SystemNotificationWithUserNameInfo = 13,
	SendAddressingSystemNotification = 14,
	AddressingSystemNotificationWithDeviceId = 15,
	AddressingSystemNotificationWithUserName = 16,
	AddressingSystemNotificationWithUserId = 17,

	UNK_1 = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	UNK_2 = 102,
};

struct NotifyBuffer
{ //Naming may be incorrect.
	NotifyType Type;		//0x00 
	int ReqId;				//0x04
	int Priority;			//0x08
	int MsgId;				//0x0C
	int TargetId;			//0x10
	int UserId;				//0x14
	int unk1;				//0x18
	int unk2;				//0x1C
	int AppId;				//0x20
	int ErrorNum;			//0x24
	int unk3;				//0x28
	char UseIconImageUri; 	//0x2C
	char Message[1024]; 	//0x2D
	char Uri[1024]; 		//0x42D
	char unkstr[1024];		//0x82D
}; //Size = 0xC30

//Device-based notification - writes directly to /dev/notification0
inline
void NotifyDevice(char* IconURI, char* MessageFMT, ...)
{
	NotifyBuffer Buffer;
	memset(&Buffer, 0, sizeof(Buffer));

	//Create full string from va list.
	va_list args;
	va_start(args, MessageFMT);
	vsprintf(Buffer.Message, MessageFMT, args);
	va_end(args);

	//Populate the notify buffer.
	Buffer.Type = NotifyType::NotificationRequest;
	Buffer.unk3 = 0;
	Buffer.UseIconImageUri = 1;
	Buffer.TargetId = -1;
	strcpy(Buffer.Uri, IconURI);

	//Open the notification device directly using SCE kernel functions
	int fd = sceKernelOpen("/dev/notification0", SCE_KERNEL_O_WRONLY, 0);
	if (fd < 0) {
		//Try the alternate device
		fd = sceKernelOpen("/dev/notification1", SCE_KERNEL_O_WRONLY, 0);
		if (fd < 0) {
			return;
		}
	}

	//Write the buffer to the device
	size_t written = sceKernelWrite(fd, &Buffer, sizeof(NotifyBuffer));
	sceKernelClose(fd);
}

