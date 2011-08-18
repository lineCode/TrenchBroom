//
//  RenderChangeSet.m
//  TrenchBroom
//
//  Created by Kristian Duske on 12.08.11.
//  Copyright 2011 TU Berlin. All rights reserved.
//

#import "RenderChangeSet.h"

@implementation RenderChangeSet

- (id)init {
    if ((self = [super init])) {
        addedEntities = [[NSMutableArray alloc] init];
        removedEntities = [[NSMutableArray alloc] init];
        changedEntities = [[NSMutableArray alloc] init];
        addedBrushes = [[NSMutableArray alloc] init];
        removedBrushes = [[NSMutableArray alloc] init];
        changedBrushes = [[NSMutableArray alloc] init];
        changedFaces = [[NSMutableArray alloc] init];
        selectedEntities = [[NSMutableArray alloc] init];
        deselectedEntities = [[NSMutableArray alloc] init];
        selectedBrushes = [[NSMutableArray alloc] init];
        deselectedBrushes = [[NSMutableArray alloc] init];
        selectedFaces = [[NSMutableArray alloc] init];
        deselectedFaces = [[NSMutableArray alloc] init];
    }
    
    return self;
}

- (void)dealloc {
    [addedEntities release];
    [removedEntities release];
    [changedEntities release];
    [addedBrushes release];
    [removedBrushes release];
    [changedBrushes release];
    [changedFaces release];
    [selectedEntities release];
    [deselectedEntities release];
    [selectedBrushes release];
    [deselectedBrushes release];
    [selectedFaces release];
    [deselectedFaces release];
    [super dealloc];
}

- (void)entitiesAdded:(NSArray *)theEntities {
    [addedEntities addObjectsFromArray:theEntities];
}

- (void)entitiesRemoved:(NSArray *)theEntities {
    [removedEntities addObjectsFromArray:theEntities];
}

- (void)entitiesChanged:(NSArray *)theEntities {
    [changedEntities addObjectsFromArray:theEntities];
}

- (void)brushesAdded:(NSArray *)theBrushes {
    [addedBrushes addObjectsFromArray:theBrushes];
}

- (void)brushesRemoved:(NSArray *)theBrushes {
    [removedBrushes addObjectsFromArray:theBrushes];
}

- (void)brushesChanged:(NSArray *)theBrushes {
    [changedBrushes addObjectsFromArray:theBrushes];
}

- (void)facesChanged:(NSArray *)theFaces {
    [changedFaces addObjectsFromArray:theFaces];
}

- (void)entitiesSelected:(NSArray *)theEntities {
    [selectedEntities addObjectsFromArray:theEntities];
}

- (void)entitiesDeselected:(NSArray *)theEntities {
    [deselectedEntities addObjectsFromArray:theEntities];
}

- (void)brushesSelected:(NSArray *)theBrushes {
    [selectedBrushes addObjectsFromArray:theBrushes];
}

- (void)brushesDeselected:(NSArray *)theBrushes {
    [deselectedBrushes addObjectsFromArray:theBrushes];
}

- (void)facesSelected:(NSArray *)theFaces {
    [selectedFaces addObjectsFromArray:theFaces];
}

- (void)facesDeselected:(NSArray *)theFaces {
    [deselectedFaces addObjectsFromArray:theFaces];
}

- (void)setFilterChanged:(BOOL)isFilterChanged {
    filterChanged = isFilterChanged;
}

- (void)clear {
    [addedEntities removeAllObjects];
    [removedEntities removeAllObjects];
    [changedEntities removeAllObjects];
    [addedBrushes removeAllObjects];
    [removedBrushes removeAllObjects];
    [changedBrushes removeAllObjects];
    [changedFaces removeAllObjects];
    [selectedEntities removeAllObjects];
    [deselectedEntities removeAllObjects];
    [selectedBrushes removeAllObjects];
    [deselectedBrushes removeAllObjects];
    [selectedFaces removeAllObjects];
    [deselectedFaces removeAllObjects];
    filterChanged = NO;
}

- (NSArray *)addedEntities {
    return addedEntities;
}

- (NSArray *)removedEntities {
    return removedEntities;
}

- (NSArray *)changedEntities {
    return changedEntities;
}

- (NSArray *)addedBrushes {
    return addedBrushes;
}

- (NSArray *)removedBrushes {
    return removedBrushes;
}

- (NSArray *)changedBrushes {
    return changedBrushes;
}

- (NSArray *)changedFaces {
    return changedFaces;
}

- (NSArray *)selectedEntities {
    return selectedEntities;
}

- (NSArray *)deselectedEntities {
    return deselectedEntities;
}

- (NSArray *)selectedBrushes {
    return selectedBrushes;
}

- (NSArray *)deselectedBrushes {
    return deselectedBrushes;
}

- (NSArray *)selectedFaces {
    return selectedFaces;
}

- (NSArray *)deselectedFaces {
    return deselectedFaces;
}

- (BOOL)filterChanged {
    return filterChanged;
}

@end