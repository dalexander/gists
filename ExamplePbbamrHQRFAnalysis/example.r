library(pbbamr)  # Get this on the cluster using "module add R/3.2.3-bauhaus"
library(dplyr)


## Use some tiny matched example files from pbcore.  In practice, the files we will want to look at
## are going to be:
##  - aligned_subreads.bam from a "fullHQ" analysis, in order to see the possible alignments
##  - the original subreads.bam (with the HQRF *on*), in order to see the HQR bounds
##
subreadsFname <- "/home/UNIXHOME/dalexander/Data/pbcore/m140905_042212_sidney_c100564852550000001823085912221377_s1_X0.subreads.bam"
alnsFname     <- "/home/UNIXHOME/dalexander/Data/pbcore/m140905_042212_sidney_c100564852550000001823085912221377_s1_X0.aligned_subreads.bam"

#

rt <- loadRegionsTable(subreadsFname) ## Load "region table",
                                      ## constructed on the fly by the
                                      ## "virtual" ZMW-read-stitcher
                                      ## in pbbam

pbi <- loadPBI(alnsFname)  ## Load the pbi "index" file for the
                           ## aligned_subreads.bam---this contains,
                           ## among other things, information about
                           ## the extents of alignments on the reads.

alnRegions <- data.frame(HoleNumber=pbi$hole,
                         RegionType=factor(c("ALIGNMENT"), levels=levels(rt$RegionType)),
                         RegionStart=pbi$astart,
                         RegionEnd=pbi$aend)

combined <- bind_rows(rt, alnRegions)

groupedByHN <- combined %>% group_by(HoleNumber)

# This is a good starting point for doing some HQRF analyses...
