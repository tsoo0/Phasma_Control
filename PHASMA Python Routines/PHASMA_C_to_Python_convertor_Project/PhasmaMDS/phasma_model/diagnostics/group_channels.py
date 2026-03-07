def group_channels(chmaps):
    diagdict = {}
    n=1;
    for i,chmap in enumerate(chmaps):
        for j in chmap.values():
            diagdict.update({n:j})
            n+=1
        
    return diagdict