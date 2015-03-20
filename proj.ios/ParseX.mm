//
//  Parse.m
//  DwarfForest
//
//  Created by Kristaps Auzins on 17/03/15.
//
//

//#import <Foundation/Foundation.h>

#import <Parse/Parse.h>
#include "ParseX.h"
//#import "Flurry.h"
//#import "AnalyticXStringUtil.h"


void ParseX::createUser() {
    //Android only
    
    cocos2d::CCLog("Create Parse User");
    
    PFUser *user = [PFUser user];
    user.username = @"my name";
    user.password = @"my pass";
    user.email = @"email@example.com";
    
    // other fields can be set if you want to save more information
    user[@"phone"] = @"650-555-0000";
    
    [user signUpInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
        if (!error) {
            // Hooray! Let them use the app now.
        } else {
            NSString *errorString = [error userInfo][@"error"];
            // Show the errorString somewhere and let the user try again.
        }
    }];
}