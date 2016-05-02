import sys, locale, os.path as op
from pbcore.io import IndexedBamReader

locale.setlocale(locale.LC_ALL, 'en_US')


def commasFmt(n):
    return locale.format("%d", n, grouping=True)

def countBases(f):
    return sum(f.qEnd - f.qStart)

root = sys.argv[1].replace(".subreads.bam", "")
subreads = IndexedBamReader(sys.argv[1])
scraps = IndexedBamReader(sys.argv[1].replace(".subreads.bam", ".scraps.bam"))


print op.basename(root)
subreadsBases = countBases(subreads)
scrapsBases = countBases(scraps)
print "  Subreads bases: " + commasFmt(subreadsBases)
print "  Scraps bases:   " + commasFmt(scrapsBases)
print "  Total bases:    " + commasFmt(subreadsBases + scrapsBases)
