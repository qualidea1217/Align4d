import numpy as np
from scipy import optimize
# evaluation script
# included metrics: WDER, JER, Text-DER(utterance level), WER

# neede more information:
# the original speaker id for hypothesis and ground-truth sequences

# (token, spk_id)
# Input to alignment tool: 
#        2 lists of tokens [hi, long, time, no, see], [Hi, long, no, see]
#        2 lists of speaker ids and utterance segments [(1,15,A), (16,50,B) ....], [(1,13,spk_0), ...]
#        Same for ground-truth and hypothesis transcripts

# output from alignment tool:
#         align each ground truth sequence to hypothesis transcripts
#         1. one csv recording all words and gaps? (need speaker information for each sequence)
#         2. [A-1, A-2, A-3, B-1, B-2, 0, 0, -1, C-1, C-2]
#             0: gap, (-1: missmatch ?) do we need it?
#         3. each speaker a list of alignment to target sequence, and a list of target sequence's id
#            [1,2,3,5,0,-1,10...], [6,7,8,9,...], [15,30,31,31,...], [spk_0,spk_0,spk_1,spk_1, ....]


def get_speaker_mapping(hyp_spk_labels, ref_spks_alignments) -> dict:
  """return a optimal speaker mapping between ref and hyp transcripts

  Args:
      hyp_spk_labels (list): a list of speaker labels for each token in the hyp sequence
      ref_spks_alignment(list): a list of multiple sequences, each stores a speakers alignment to target sequence
  Returns:
      dict: map from ref to hyp
  """
  # TODO: some speakers in ref might aligned to the same speaker in hyp, compare between speakers
  hyp_spk_set = set(hyp_spk_labels)
  reference_speaker = 0
  speaker_alignment = {}
  for aligned_indices in ref_spks_alignments: # iterate throught each speaker sequence's aligned result
    max_matched_indices = 0
    for hyp_spk in hyp_spk_set:
      hyp_indices = [index for (index, item) in enumerate(hyp_spk_labels) if item == hyp_spk] 
      matched_indices = len(set(aligned_indices) & set(hyp_indices))
      if matched_indices > max_matched_indices:
        max_matched_indices = matched_indices
        speaker_alignment[hyp_spk] = reference_speaker
    reference_speaker += 1 # next reference speakers

  return speaker_alignment

      
      

def get_spk_set(spk_labels):
  """_summary_

  Args:
      spk_labels (list): a list stores each token's speaker label for

  Returns:
      set: a set with all speaker ids
  """
  spk_set = set()
  return spk_set

def get_hyp_aligned_refSpearkerLables(ref_spks_alignments, hyp_length) -> list:
  """having the aligned sequence for each ref speaker, return the list of ref speaker labels for each hyp token
  
  Args:
    ref_spks_alignments (list): a list of lists. Each list recordes the alignment from ref speaker sequence to target sequence
    hyp_length (int): the length of hyp sequence
  """
  hyp_aligned_ref_spkIDs = [-1 for i in range(hyp_length)]
  for ref_spkID,aligned_indices in enumerate(ref_spks_alignments):
    for index in aligned_indices:
      if index != -1:
        hyp_aligned_ref_spkIDs[index] = ref_spkID
  
  return hyp_aligned_ref_spkIDs
    

def WDER(hyp_spk_labels, ref_spks_alignments):
  """Word diarization error rate:
  WDER = (Sis + Cis)/(S+C) 
  the error rate amoung aligned tokens (not including gaps)
  """
  # TODO: -1/0 stands for gap here, should we also account for missmatch
  speaker_alignment = get_speaker_mapping(hyp_spk_labels=hyp_spk_labels, ref_spks_alignments=ref_spks_alignments)
  hyp_aligned_ref_spkIDs = get_hyp_aligned_refSpearkerLables(ref_spks_alignments=ref_spks_alignments, hyp_length=len(hyp_spk_labels))
  incorrect_num = 0
  for index, ref_spk in enumerate(hyp_aligned_ref_spkIDs):
    if ref_spk != -1 and speaker_alignment[ref_spk] != hyp_spk_labels[index]:
      incorrect_num += 1
  
  WDER_score = incorrect_num/len([spk for spk in hyp_spk_labels if spk != -1])
      
  return WDER_score



if __name__ == "__main__":
  pass


