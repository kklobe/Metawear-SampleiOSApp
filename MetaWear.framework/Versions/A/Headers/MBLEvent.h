/**
 * MBLEvent.h
 * MetaWear
 *
 * Created by Stephen Schiffli on 10/8/14.
 * Copyright 2014 MbientLab Inc. All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user who
 * downloaded the software, his/her employer (which must be your employer) and
 * MbientLab Inc, (the "License").  You may not use this Software unless you
 * agree to abide by the terms of the License which can be found at
 * www.mbientlab.com/terms . The License limits your use, and you acknowledge,
 * that the  Software may not be modified, copied or distributed and can be used
 * solely and exclusively in conjunction with a MbientLab Inc, product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
 *
 * YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 * PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 * NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 * MBIENTLAB OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE,
 * STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
 * THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED
 * TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST
 * PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
 * SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY
 * DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 * Should you have any questions regarding your right to use this Software,
 * contact MbientLab Inc, at www.mbientlab.com.
 */

#import <MetaWear/MBLConstants.h>

typedef enum {
    MBLComparisonOperationEqual = 0,
    MBLComparisonOperationNotEqual = 1,
    MBLComparisonOperationLessThan = 2,
    MBLComparisonOperationLessThanOrEqual = 3,
    MBLComparisonOperationGreaterThan = 4,
    MBLComparisonOperationGreaterThanOrEqual = 5
} MBLComparisonOperation;

/**
 @class MBLEvent
 @discussion This object represents general "events" that modules on the MetaWear board generate.
 There are several things you can do when an event occurs, all of which are programmable using
 this object, they are:
 1) Send notifications to the connected iOS device when the event occurs, see
    startNotificationsWithHandler: and stopNotifications
 2) Program other commands to be executed offline on the MetaWear device when the event occurs,
    see programCommandsToRunOnEvent: and eraseCommandsToRunOnEvent.
 3) Log the event in the MetaWear's flash storage, see startLogging, stopLogging, and 
    downloadLogWithHandler:progressHandler:
 
 Consider the switch update event (mechanicalSwitch.switchUpdateEvent), if you call 
 startNotificationsWithHandler: on that MBLEvent and keep a live connection to the MetaWear, 
 any time you press or release the switch you will get a callback to the provided block.  If you
 call startLogging then anytime you press or release the button, an entry will be created in the
 log which can be download later using downloadLogWithHandler:progressHandler:.  If you want
 the device to buzz when you press the switch then you would call programCommandsToRunOnEvent:
 and call [device.hapticBuzzer startHapticWithDutyCycle:248 pulseWidth:500 completion:nil] in the
 provided block.
 */
@interface MBLEvent : NSObject

/**
 Start receiving callbacks when this event occurs. The type of the
 object that is passed to the handler depends on the event being handled
 @returns none
 */
- (void)startNotificationsWithHandler:(MBLObjectHandler)handler;
/**
 Stop receiving callbacks when this event occurs
 @returns none
 */
- (void)stopNotifications;


/**
 This function is used for programing the Metawear device to perform actions
 automatically.  Any time this even occurs you can have it trigger other
 Metawear API calls even when the phone isn't connected.
 When this function is called, the given block executed and checked for
 validity.  All Metawear API calls inside the block are sent to the device
 for execution later.  THE BLOCK IS ONLY EXECUTED ONCE DURNING THIS CALL AND
 NEVER AGAIN, DON'T ATTEMPT TO USE CALLBACKS INSIDE THIS BLOCK
 @param void(^)() block, Block consisting of API calls to make when this event occus
 @returns none
 */
- (void)programCommandsToRunOnEvent:(void(^)())block;
/**
 Removes all commands setup when calling programCommandsToRunOnEvent:
 @returns none
 */
- (void)eraseCommandsToRunOnEvent;


/**
 Start recording notifications for this event.  Each time this event occus
 an entry is made into non-volatile flash memory that is on the metawear device.
 This is useful for tracking things while the phone isn't connected to the Metawear
 @returns none
 */
- (void)startLogging;
/**
 Stop recording notifications for this event.
 */
- (void)stopLogging;
/**
 Fetch contents of log from MetaWear device.  Executes the progressHandler
 periodically with the progress (0.0 - 1.0), progressHandler will get called
 with 1.0 before handler is called.  The handler is passed an array of entries,
 the exact class of the entry depends on what is being logged.  For example,
 the accelerometer log returns an array of MBLAccelerometerData's
 @param MBLDataHandler handler, Callback once download is complete
 @param MBLFloatHandler progressHandler, Periodically called while log download is in progress
 @returns none
 */
- (void)downloadLogWithHandler:(MBLArrayErrorHandler)handler
               progressHandler:(MBLFloatHandler)progressHandler;

@end
