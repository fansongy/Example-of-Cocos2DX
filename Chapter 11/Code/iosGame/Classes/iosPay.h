//
//  iosPay.h
//  iosGame
//
//  Created by Fansy on 14-4-6.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@class ViewController;
@interface iosPay : NSObject<SKPaymentTransactionObserver, SKProductsRequestDelegate>

@property (nonatomic, readwrite, strong) ViewController* viewController;
+ (iosPay *)getInstance;

- (void)initStoreKit;
- (void)purchaseItem: (NSString*)identifier;

@end
