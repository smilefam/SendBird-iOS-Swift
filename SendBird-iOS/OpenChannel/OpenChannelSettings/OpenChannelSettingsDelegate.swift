//
//  OpenChannelSettingsDelegate.swift
//  SendBird-iOS
//
//  Created by Jed Gyeong on 10/22/18.
//  Copyright © 2018 SendBird. All rights reserved.
//

import Foundation

@objc protocol OpenChannelSettingsDelegate: NSObjectProtocol {
    @objc optional func didUpdateOpenChannel()
}
