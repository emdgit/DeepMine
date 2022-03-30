from math import floor
import string
import ruamel.yaml as ry
import re

class Location:
    def __init__(self, num=0, hasOil=False) -> None:
        self.number_ = num
        self.has_oil_ = hasOil
        self.partitions_ = {}
        self.extra_ = {}

    @property 
    def number(self):
        return self.number_

    def addPartition(self, resId, prob):
        if resId in self.partitions_.keys():
            print("{} is already in Partitions list")
            return
        self.partitions_[resId] = prob

    def addExtra(self, resIdList, ratioStr):
        if len(self.extra_) != 0:
            raise Exception("Extra already added")
        
        n = len(resIdList)
        pattern = ""
        
        for _ in range(n):
            pattern = pattern + "(\d+)-"
        pattern = pattern[:-1]
        
        extraVals = [0 for _ in range(n)]
        extraAll = 0

        matches = re.findall("\({}\)".format(pattern), ratioStr)

        if len(matches) == 0:
            print("Nothing extra found for location #{}".format(self.number))
            return

        for match in matches:
            if len(match) != n:
                raise Exception("Invalid ratio string")
            for g in range(n):
                extraVals[g] += int(match[g])
                extraAll += int(match[g])
        
        for i in range(n):
            self.extra_[resIdList[i]] = round(extraVals[i] * 100 / extraAll)

loc = Location(1, False)
loc.addExtra([1,2,3,13], "(1-22-33) (1-22-33-444) (4-55-7)")

doc = ry.load(open("schema.yaml", "r"), Loader=ry.Loader)

# id / res name
resources = {}
locations = []

for res in doc["Resources"]:
    resources[res["id"]] = res["name"]

for loc in doc["Locations"]:
    num = loc["number"]
    oil = "oil" in loc.keys()
    location = Location(num, oil)
    locations.append(location)
