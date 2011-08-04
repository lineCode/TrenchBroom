//
//  MapBrowserDataSource.m
//  TrenchBroom
//
//  Created by Kristian Duske on 03.08.11.
//  Copyright 2011 TU Berlin. All rights reserved.
//

#import "MapBrowserDataSource.h"
#import "Map.h"
#import "Entity.h"
#import "Brush.h"
#import "Face.h"

@implementation MapBrowserDataSource

- (void)setMapWindowController:(MapWindowController *)theMapWindowController {
    mapWindowController = theMapWindowController;
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
    if (item == nil) {
        return [mapWindowController document];
    } if ([item conformsToProtocol:@protocol(Map)]) {
        id <Map> map = item;
        return [[map entities] objectAtIndex:index];
    } else if ([item conformsToProtocol:@protocol(Entity)]) {
        id <Entity> entity = item;
        return [[entity brushes] objectAtIndex:index];
    } else if ([item conformsToProtocol:@protocol(Brush)]) {
        id <Brush> brush = item;
        return [[brush faces] objectAtIndex:index];
    }
    [NSException raise:NSInvalidArgumentException format:@"unknown item: %@", item];
    return nil;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
    if ([item conformsToProtocol:@protocol(Map)]) {
        id <Map> map = item;
        return [[map entities] count] > 0;
    } else if ([item conformsToProtocol:@protocol(Entity)]) {
        id <Entity> entity = item;
        return [[entity brushes] count] > 0;
    } else if ([item conformsToProtocol:@protocol(Brush)]) {
        id <Brush> brush = item;
        return [[brush faces] count] > 0;
    }
    return NO;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
    if (item == nil) {
        return 1;
    } if ([item conformsToProtocol:@protocol(Map)]) {
        id <Map> map = item;
        return [[map entities] count];
    } else if ([item conformsToProtocol:@protocol(Entity)]) {
        id <Entity> entity = item;
        return [[entity brushes] count];
    } else if ([item conformsToProtocol:@protocol(Brush)]) {
        id <Brush> brush = item;
        return [[brush faces] count];
    }
    return 0;
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
    if ([item conformsToProtocol:@protocol(Map)]) {
        id <Map> map = item;
        id <Entity> worldspawn = [map worldspawn:NO];
        NSString* message = [worldspawn propertyForKey:MessageKey];
        return message != nil ? [NSString stringWithFormat:@"Map (%@)", message] : @"Map";
    } else if ([item conformsToProtocol:@protocol(Entity)]) {
        id <Entity> entity = item;
        id <Map> map = [entity map];
        return [NSString stringWithFormat:@"Entity %i (%@)", [[map entities] indexOfObject:entity], [entity classname]];
    } else if ([item conformsToProtocol:@protocol(Brush)]) {
        id <Brush> brush = item;
        id <Entity> entity = [brush entity];
        return [NSString stringWithFormat:@"Brush %i", [[entity brushes] indexOfObject:brush]];
    } else if ([item conformsToProtocol:@protocol(Face)]) {
        id <Face> face = item;
        id <Brush> brush = [face brush];
        return [NSString stringWithFormat:@"Face %i", [[brush faces] indexOfObject:face]];
    }
    [NSException raise:NSInvalidArgumentException format:@"unknown item: %@", item];
    return nil;
}

@end