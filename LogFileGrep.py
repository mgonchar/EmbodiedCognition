'''

@author: m1r
'''

import re
import numpy
from os import listdir
from os.path import isfile, join

def ExtractInfo(pth):
    active_side = 'right'
    current_category = 'hand'
    
    think_time = 0
    move_time = 0
    
    perif_circle_delay = 332
    
    think_time_left_leg = []
    think_time_left_hand = []
    think_time_right_leg = []
    think_time_right_hand = []
    
    move_time_left_leg = []
    move_time_left_hand = []
    move_time_right_leg = []
    move_time_right_hand = []
    
    total_time_left_leg = []
    total_time_left_hand = []
    total_time_right_leg = []
    total_time_right_hand = []
    
    trials_failed_left_common = 0
    trials_failed_left_leg = 0
    trials_failed_left_hand = 0
    trials_failed_right_common = 0
    trials_failed_right_leg = 0
    trials_failed_right_hand = 0
    
    flnm = pth.split('/')[-1]
    
    date = flnm.split('_')[-1]
    date = date[:-4]
    
    info = flnm.split('_')[:-1]
    
    if 'hand' in info:
        limb = 'hand'
    else:
        limb = 'leg'
        
    if 'colored' in info:
        series_type = 'colored'
    else:
        series_type = 'ordinal'
        
    info.remove(limb)
    info.remove(series_type)
    
    sbj_id = "_".join(info)
    
    fread = open(pth, 'rb').read()
    mytext = fread.decode('utf-16')
    mytext = mytext.encode('ascii', 'ignore')
    fwrite = open('./tmp.log', 'wb')
    fwrite.write(mytext)
    
    #fread.close()
    fwrite.close()
    
    f = open('./tmp.log','r')
    
    for line in f:
        if (line.find("Active hand:") != -1):
            if (line.find("RIGHT") != -1):
                active_side = 'right'
            
            if (line.find("LEFT") != -1):
                active_side = 'left'
            continue
    
        if (line.find("Displaying text:") != -1):
            if (line.find("HAND") != -1):
                current_category = 'hand'
                
            if (line.find("LEG") != -1):
                current_category = 'leg'
                
            if (line.find("COMMON") != -1):
                current_category = 'common'
            continue
    
        if (line.find("Thinked before action:") != -1):
            think_time = int(re.findall(r'\d+', line)[0])
            
        if (line.find("Finger movement taken:") != -1):
            move_time = int(re.findall(r'\d+', line)[0])
            
            if(current_category == 'hand'):
                if (active_side == 'left'):
                    think_time_left_hand.append(think_time)
                    move_time_left_hand.append(move_time)
                    total_time_left_hand.append(move_time+think_time)
                else:
                    think_time_right_hand.append(think_time)
                    move_time_right_hand.append(move_time)
                    total_time_right_hand.append(move_time+think_time)
                continue
            
            if(current_category == 'leg'):
                if (active_side == 'left'):
                    think_time_left_leg.append(think_time)
                    move_time_left_leg.append(move_time)
                    total_time_left_leg.append(move_time+think_time)
                else:
                    think_time_right_leg.append(think_time)
                    move_time_right_leg.append(move_time)
                    total_time_right_leg.append(move_time+think_time)
                continue
        
        if (line.find("Trial failed") != -1):
            if(current_category == 'hand'):
                if (active_side == 'left'):
                    trials_failed_left_hand += 1
                else:
                    trials_failed_right_hand += 1
                continue
            
            if(current_category == 'leg'):
                if (active_side == 'left'):
                    trials_failed_left_leg += 1
                else:
                    trials_failed_right_leg += 1
                continue
            
            if(current_category == 'common'):
                if (active_side == 'left'):
                    trials_failed_left_common += 1
                else:
                    trials_failed_right_common += 1
                continue
         
    f.close()
    
    print("\n\nExperiment date: \t" + date + "\n")
    print("Subdject: \t" + sbj_id + "\n")
    print("Series type: \t" + series_type + "\n")
    print("Tested: \t" + limb + "\n")
    print("\nResults:\n")
    print("\tLEFT:\n")
    print("\t\t# errors leg-words\t" + str(trials_failed_left_leg) + "\n")
    print("\t\t# errors hand-words\t" + str(trials_failed_left_hand) +"\n")
    print("\t\t# errors common-words\t" + str(trials_failed_left_common) +"\n")
    print("\t\t Median reaction time leg-words\t" + str(numpy.median(total_time_left_leg)) +"ms\n")
    print("\t\t Median reaction time hand-words\t" + str(numpy.median(total_time_left_hand)) +"ms\n")
    print("\t\t Mean reaction time leg-words (Italiano)\t" + str(numpy.mean(think_time_left_leg) - perif_circle_delay) +"ms\n")
    print("\t\t Mean reaction time hand-words (Italiano)\t" + str(numpy.mean(think_time_left_hand) - perif_circle_delay) +"ms\n")
    print("\tRIGHT:\n")
    print("\t\t# errors leg-words\t" + str(trials_failed_right_leg) + "\n")
    print("\t\t# errors hand-words\t" + str(trials_failed_right_hand) +"\n")
    print("\t\t# errors common-words\t" + str(trials_failed_right_common) +"\n")
    print("\t\t Median reaction time leg-words\t" + str(numpy.median(total_time_right_leg)) +"ms\n")
    print("\t\t Median reaction time hand-words\t" + str(numpy.median(total_time_right_hand)) +"ms\n")
    print("\t\t Mean reaction time leg-words (Italiano)\t" + str(numpy.mean(think_time_right_leg) - perif_circle_delay) +"ms\n")
    print("\t\t Mean reaction time hand-words (Italiano)\t" + str(numpy.mean(think_time_right_hand) - perif_circle_delay) +"ms\n")


dir_path = 'C:/Users/m1r/Documents/EmbodiedCognitionExpirement/release/subjects/logs/app_v2/'
onlyfiles = [f for f in listdir(dir_path) if isfile(join(dir_path, f))]

for sbj_log in onlyfiles:
    ExtractInfo(dir_path + sbj_log)
#pth = 'C:/Users/m1r/Documents/EmbodiedCognitionExpirement/release/subjects/vasili_ordinal_hand_12.09.2016.log'


tt = 0
        