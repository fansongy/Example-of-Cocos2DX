//
//  iosPay.m
//  iosGame
//
//  Created by Fansy on 14-4-6.
//
//

#import "iosPay.h"
#import "HelloWorldScene.h"

@implementation iosPay

+ (iosPay *)getInstance
{
	static iosPay *pay = nil;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		pay = [[super alloc] init];
		[pay initStoreKit];
	});
    
	return pay;
}

- (void)initStoreKit
{
	[[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

- (void)purchaseItem: (NSString *)identifier
{
	SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: identifier]];
	request.delegate = self;
	[request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	NSArray *myProduct = response.products;
	
	if (myProduct.count == 0) {
		NSLog(@"Fail Can't find Product invalidProductIdentifiers = %@",response.invalidProductIdentifiers);
		return;
	}
    
	NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
	if (transactions.count > 0) {
		NSLog(@"rePurchase...");
		SKPaymentTransaction* transaction = [transactions firstObject];
		if (transaction.transactionState == SKPaymentTransactionStatePurchased) {
			[self completeTransaction:transaction];
			[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
			return;
		}
	}
	
	NSLog(@"Purchase...");
	SKPayment * payment = [SKPayment paymentWithProduct:myProduct[0]];
	[[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	NSLog(@"receive call back");
	for (SKPaymentTransaction *transaction in transactions){
		switch (transaction.transactionState){
			case SKPaymentTransactionStatePurchased:
				[self completeTransaction:transaction];
				break;
				
			case SKPaymentTransactionStateFailed:
                NSLog(@"Fail error.code:%d",(int)transaction.error.code);
                [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
				break;
				
			case SKPaymentTransactionStateRestored:
                NSLog(@"restored");
                [self completeTransaction: transaction];
				break;
			default:
				break;
		}
	}
}
- (void) completeTransaction: (SKPaymentTransaction*)transaction
{
	NSLog(@"Success");
    
    NSString* iden = transaction.transactionIdentifier;
	std::string idens = [iden UTF8String];
    
    auto current = cocos2d::Director::getInstance()->getRunningScene();
    for(auto layer : current->getChildren())
    {
        auto hello = dynamic_cast<HelloWorld*>(layer);
        if(hello)
        {
            hello->setInfo(idens);
        }
    }

	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

@end
