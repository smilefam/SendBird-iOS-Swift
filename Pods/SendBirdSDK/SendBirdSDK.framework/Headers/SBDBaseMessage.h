//
//  SBDBaseMessage.h
//  SendBirdSDK
//
//  Created by Jed Gyeong on 5/30/16.
//  Copyright © 2016 SENDBIRD.COM. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SBDMessageListParams.h"
#import "SBDThreadInfo.h"
#import "SBDUser.h"
#import "SBDReaction.h"
#import "SBDReactionEvent.h"
#import "SBDThreadedMessageListParams.h"
#import "SBDThreadInfoUpdateEvent.h"
#import "SBDMessageRetrievalParams.h"

NS_ASSUME_NONNULL_BEGIN

@class SBDBaseChannel, SBDMessageMetaArray, SBDSender;
@class SBDOGMetaData;


/// The `SBDBaseMessage` class represents the base message which is generated by a user or an admin.
/// The `SBDUserMessage`, the `SBDFileMessage` and the `SBDAdminMessage` are derived from this class.
@interface SBDBaseMessage : NSObject <NSCopying>

/// Request ID for checking ACK.
@property (strong, nonatomic, readonly) NSString *requestId;

/// Unique message ID.
@property (atomic) long long messageId;

/// Message text.
@property (strong, nonatomic, readonly) NSString *message;

/// Represents the dispatch state of the message.
/// If message is not dispatched completely to the Sendbird server, the value is `SBDMessageSendingStatusPending`.
/// If failed to send the message, the value is `SBDMessageSendingStatusFailed`.
/// And if success to send the message, the value is `SBDMessageSendingStatusSucceeded`.
/// @since 3.0.173
@property (assign, nonatomic, readonly) SBDMessageSendingStatus sendingStatus;

/// Sender of the <span>message</span>. This is represented by `SBDSender` class.
@property (strong, nonatomic, nullable) SBDSender *sender;

/// Channel URL which has this message.
@property (strong, nonatomic) NSString *channelUrl;

/// Channel type of this message.
@property (strong, nonatomic) NSString *channelType;

/// The list of users who was mentioned together with the message.
/// @since 3.0.90
@property (strong, nonatomic, readonly) NSArray <SBDUser *> *mentionedUsers;

/// @since 3.0.103
@property (atomic, readonly) SBDMentionType mentionType;

/// Represents target user ids to mention when success to send the message.
/// This value is valid only when the message is a pending message or failed message.
/// If the message is a succeeded message, see `mentionedUserIds`
/// @since 3.0.147
/// @see see `mentionedUserIds` when the message is a succeeded message.
@property (strong, nonatomic, readonly) NSArray<NSString *> *requestedMentionUserIds;

/// Message created time in millisecond(UTC).
@property (atomic) long long createdAt;

/// Message updated time in millisecond(UTC).
@property (atomic) long long updatedAt;

/// The unique ID of the parent message. If the message object is a parent message or a single message without any reply, the value of this property is 0. If the object is a reply, the value is the unique ID of its parent message.
/// @note The default value is `0`.
/// @since 3.0.181
@property (atomic, readonly) long long parentMessageId;

/// The written text of the message object’s parent message. If the message object is a parent message, the value of this property is `nil`. If the object is a reply to a parent message and the type of the parent message is [`SBDUserMessage`](../Classes/SBDUserMessage.html), the value is [`message`](../Classes/SBDUserMessage.html#//api/name/message). If it is [`SBDFileMessage`](../Classes/SBDFileMessage.html), the value is the [`name`](../Classes/SBDFileMessage.html#//api/name/name) of the uploaded file.
/// @since 3.0.181
@property (strong, readonly, nullable) NSString *parentMessageText;

/// The thread info that belongs to this message object.
/// @since 3.0.181
@property (strong, readonly) SBDThreadInfo *threadInfo;

///  Meta array is key-value property, a key is a string and a value is an array with a string.
/// ex. @{@"key1":@[@"str1", @"str2", ... ], @"key2":@[@"str3", @"str4", ... ]}
/// If you want to get a sorted meta array, refer to -metaArrays
///
///  @since 3.0.116
///  @deprecated 3.0.148
@property (nonatomic, readonly, getter=getAllMetaArray) NSDictionary<NSString *, NSArray<NSString *> *> *metaArray
DEPRECATED_ATTRIBUTE;

/// Gets an array of meta arrays sorted by chronological order.
///
/// @since 3.0.148
@property (strong, nonatomic, readonly, nullable) NSArray<SBDMessageMetaArray *> *metaArrays;

/// The custom data for message.
/// @see Moved from `SBDUserMessage`, `SBDFileMessage`, `SBDAdminMessage`.
@property (strong, nonatomic, readonly) NSString *data;

/// Custom message type.
/// @see Moved from `SBDUserMessage`, `SBDFileMessage`, `SBDAdminMessage`.
@property (strong, nonatomic, readonly, nullable) NSString *customType;

@property (strong, nonatomic, readonly, getter=getReactions) NSArray<SBDReaction *> *reactions;

@property (atomic, readonly) NSInteger messageSurvivalSeconds;

/// Whether the message is silent or not
/// @note If the message is silent,  last message won't be changed and `channelWasChanged:` will not get called on receiver's sidel
/// @since 3.0.184
@property (atomic, readonly, getter=isSilent) BOOL silent;

/// The error code of file.
/// This value generated only when message send fails.
/// @since 3.0.161
@property (assign, nonatomic, readonly) NSUInteger errorCode;

/// Optional open graph information if the message contains links.
/// We only support first open graph tag appearance on the first link
/// @since 3.0.193
@property (nonatomic, strong, readonly, nullable) SBDOGMetaData *ogMetaData;

/// Checks the channel type is open channel or not.
///
/// @return Returns YES, when this is open channel.
- (BOOL)isOpenChannel;

/// Checks the channel type is group channel or not.
///
/// @return Returns YES, when this is group channel.
- (BOOL)isGroupChannel;

/// Checks message can resend.
/// The message can only resend if the problem is due to network related error.
///
/// @return If YES, this message can resend.
/// @since 3.0.161
- (BOOL)isResendable;

/// Builds a message object from serialized <span>data</span>.
///
///  @param data Serialized <span>data</span>.
///  @return SBDBaseMessage object.
+ (nullable instancetype)buildFromSerializedData:(NSData * _Nonnull)data;

///  Serializes message object.
///
///  @return Serialized <span>data</span>.
- (nullable NSData *)serialize;

///  Returns meta array for the keys.
///
///  @param keys Keys of the meta array.
///  @return Meta array of the keys.
///  @see -metaArraysWithKeys:
///  @since 3.0.116
///  @deprecated 3.0.148
- (NSDictionary<NSString *, NSArray<NSString *> *> *)getMetaArrayWithKeys:(NSArray<NSString *> * _Nonnull)keys DEPRECATED_MSG_ATTRIBUTE("Use -metaArraysWithKeys:");

///  Returns an array of meta arrays for keys.
///
///  @param keys Keys of the meta array.
///  @return An Array of meta array with keys.
///  @since 3.0.148
///
///  @code
///     NSArray<NSString *> *keys = meta_array_keys;
///     NSArray<SBDMessageMetaArray *> *subMetaArrays = [message metaArraysWithKeys:keys];
///  @endcode
- (NSArray<SBDMessageMetaArray *> *)metaArraysWithKeys:(nonnull NSArray<NSString *> *)keys;

- (BOOL)applyReactionEvent:(nonnull SBDReactionEvent *)reactionEvent;

/// Retrieves a message with a specified message ID.
/// @param params Contains a set of parameters you can set regarding the messages in the results.
/// @param completionHandler The handler block to be executed. The `message` retrieves the message with the matching message ID. A value of `nil` indicates that there is no message with the specified message ID. The `error` Indicates whether there is an error. If there is no error, the value is `nil`.
/// @since 3.0.181
+ (void)getMessageWithParams:(nonnull SBDMessageRetrievalParams *)params
           completionHandler:(nullable void (^)(SBDBaseMessage * _Nullable message, SBDError * _Nullable error))completionHandler;

/// Retrieves the threaded replies of the current message depending on the timestamp. If the current message doesn’t have replies, the result is `nil`.
/// @param timestamp Specifies the timestamp to be the reference point of the retrieval, in Unix milliseconds format.
/// @param params Contains a set of parameters you can set regarding the messages in the results.
/// @param completionHandler The handler block to be executed. The `parentMessage` retrieves the parent message of the thread. The `threadedReplies` retrieves the threaded replies in the thread. The `error` indicates whether there is an error. If there is no error, the value is `nil`.
/// @since 3.0.181
- (void)getThreadedMessagesByTimestamp:(long long)timestamp
                                params:(nonnull SBDThreadedMessageListParams *)params
                     completionHandler:(nullable void (^)(SBDBaseMessage * _Nullable parentMessage,
                                                          NSArray<SBDBaseMessage *> * _Nullable threadedReplies,
                                                          SBDError * _Nullable error))completionHandler;

/// Applies the update of the thread information to the message object. This method has to be called when the [`channel:didUpdateThreadInfo:`](../Protocols/SBDChannelDelegate.html#//api/name/channel:didUpdateThreadInfo:) event returns the [`SBDThreadInfoUpdateEvent`](../Classes/SBDThreadInfoUpdateEvent.html) object.
/// @param threadInfoUpdateEvent The event object that is applied to.
/// @return Returns If the `threadInfoUpdateEvent` parameter is `nil` or the [`targetMessageId`](../Classes/SBDThreadInfoUpdateEvent.html#//api/name/targetMessageId) included in the parameter doesn’t match any message, this method returns `NO`.
/// @since 3.0.181
- (BOOL)applyThreadInfoUpdateEvent:(nonnull SBDThreadInfoUpdateEvent *)threadInfoUpdateEvent;

@end

NS_ASSUME_NONNULL_END
