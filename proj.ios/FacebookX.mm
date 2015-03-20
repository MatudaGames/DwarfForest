//
//  FacebookX.cpp
//  DwarfForest
//
//  Created by Kristaps Auzins on 17/03/15.
//
//

#include "FacebookX.h"
#import <FacebookSDK/FacebookSDK.h>
#import <ParseFacebookUtils/PFFacebookUtils.h>
#import <Parse/Parse.h>
//#import "AppController.h"

void FacebookX::logInUser() {
    /*
    FBLoginView *loginView = [[FBLoginView alloc] init];
    loginView.center = [UIApplication sharedApplication].keyWindow.rootViewController.view.center;
    [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:loginView];
    */
    
    if ([PFUser currentUser] && // Check if user is cached
        [PFFacebookUtils isLinkedWithUser:[PFUser currentUser]]) { // Check if user is linked to Facebook
        // Present the next view controller without animation
        
        [[PFUser currentUser] refresh];
        NSLog(@"User is cached!!!");
        loadData();
    }
    else{
        NSArray *permissionsArray = @[ @"public_profile", @"email", @"user_friends"];
        
        [PFFacebookUtils logInWithPermissions:permissionsArray block:^(PFUser *user, NSError *error) {
            
            if (!user) {
                NSString *errorMessage = nil;
                if (!error) {
                    NSLog(@"Uh oh. The user cancelled the Facebook login.");
                    errorMessage = @"Uh oh. The user cancelled the Facebook login.";
                } else {
                    NSLog(@"Uh oh. An error occurred: %@", error);
                    errorMessage = [error localizedDescription];
                }
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Log In Error"
                                                                message:errorMessage
                                                               delegate:nil
                                                      cancelButtonTitle:nil
                                                      otherButtonTitles:@"Dismiss", nil];
                [alert show];
            } else {
                if (user.isNew) {
                    NSLog(@"User with facebook signed up and logged in!");
                } else {
                    NSLog(@"User with facebook logged in!");
                }
                
//                loadData();
            }
            
        }];
    }
}

void FacebookX::loadData() {
    
//    PFUser *user = [PFUser currentUser];
    
    NSString *useLevel = [PFUser currentUser] [@"GameScoreID"];
    NSLog(@"User GameScoreID %@:",useLevel);
    
    PFQuery *query = [PFQuery queryWithClassName:@"GameScore"];
    
    if(useLevel != NULL && [query getObjectWithId:useLevel] != NULL)
    {
        NSLog(@"User GameScoreID IS NOT NULL - UPDATE OLD");
        // We have old
        [query getObjectInBackgroundWithId:useLevel block:^(PFObject *gameScore, NSError *error) {
            
            // Now let's update it with some new data. In this case, only cheatMode and score
            // will get sent to the cloud. playerName hasn't changed.
            gameScore[@"CurrentLevel"] = @100;
            [gameScore saveInBackground];
        }];
    }
    else
    {
        NSLog(@"User GameScoreID IS NULL - CREATE NEW");
        // Create new
        NSNumber *number = @42;
        NSNumber *number2 = @80;
        NSArray *array = @[number2, number];
        
        PFObject *gameScore = [PFObject objectWithClassName:@"GameScore"];
        gameScore[@"CurrentLevel"] = number;
        gameScore[@"LevelPoints"] = array;
        
        [gameScore saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
            if (succeeded) {
                // The object has been saved.
                NSLog(@"The new data ID: %@",[gameScore objectId]);
                
                [PFUser currentUser][@"GameScoreID"] = [gameScore objectId];
                [[PFUser currentUser] saveInBackground];
            } else {
                // There was a problem, check error.description
            }
        }];
    }
    

    return;
    

    
    /*
    PFQuery *query = [PFQuery queryWithClassName:@"User"];
    [query getObjectInBackgroundWithId:@"QcztRWC3aF" block:^(PFObject *gameScore, NSError *error) {
        // Do something with the returned PFObject in the gameScore variable.
        NSLog(@"%@", gameScore);
    }];
    */
    
    return;
    
    [FBRequestConnection startForMyFriendsWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
        
        if (!error && result)
        {
            NSLog(@"Good we have friends!");
            NSArray* fetchedFriendData = [[NSArray alloc] initWithArray:[result objectForKey:@"data"]];
//            callback(fetchedFriendData);
            
            NSDictionary *friendData = [fetchedFriendData objectAtIndex: arc4random() % fetchedFriendData.count];
            
            NSString *friendId = [friendData objectForKey:@"id"];
            
            NSLog(@"Friend ID:%@",friendId);
            NSLog(@"Friend Name:%@",[friendData objectForKey:@"name"]);
            
//            m_friendName = [[[fetchedFriendData objectForKey:@"name"] componentsSeparatedByString:@" "] objectAtIndex:0];
//            
//            m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@!", m_friendName];
//            
//            if (m_pFriendTexture) {
//                delete m_pFriendTexture;
//            }
//            
//            m_uFriendFBID = [friendId longLongValue];
//            
//            m_pFriendTexture = new System::TextureResource();
//            m_pFriendTexture->CreateFromFBID(m_uFriendFBID, 128, 128);
        }
        else
        {
            NSLog(@"Error no friends!");
//            callback(nil);
        }
        
    }];
    
    return;
    
    // ...
    FBRequest *request = [FBRequest requestForMe];
    [request startWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
        if (!error) {
            // result is a dictionary with the user's Facebook data
            NSDictionary *userData = (NSDictionary *)result;
            
            NSString *facebookID = userData[@"id"];
            NSString *name = userData[@"name"];
//            NSString *location = userData[@"location"][@"name"];
            NSString *gender = userData[@"gender"];
//            NSString *birthday = userData[@"birthday"];
//            NSString *relationship = userData[@"relationship_status"];
            
            NSURL *pictureURL = [NSURL URLWithString:[NSString stringWithFormat:@"https://graph.facebook.com/%@/picture?type=large&return_ssl_resources=1", facebookID]];
            
            // Now add the data to the UI elements
            // ...
            
            NSLog(@"Facebook User info ID:%@ |Name:%@ |gender:%@ |pictureURL:%@",facebookID,name,gender,pictureURL);
            
            // Try to load the pic
            // URL should point to https://graph.facebook.com/{facebookId}/picture?type=large&return_ssl_resources=1
            
            NSURLRequest *urlRequest = [NSURLRequest requestWithURL:pictureURL];
            
            // Run network request asynchronously
            [NSURLConnection sendAsynchronousRequest:urlRequest
                                               queue:[NSOperationQueue mainQueue]
                                   completionHandler:
             ^(NSURLResponse *response, NSData *data, NSError *connectionError) {
                 if (connectionError == nil && data != nil) {
                     // Set the image in the header imageView
//                     self.headerImageView.image = [UIImage imageWithData:data];
                     
                     UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, 256, 256)];
                     imageView.image = [UIImage imageWithData:data];
                     [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:imageView];
                     
//                     [[UIApplication sharedApplication].keyWindow.rootViewController.view adds];
                 }
             }];
        }
    }];
}
