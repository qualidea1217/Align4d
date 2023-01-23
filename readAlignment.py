import csv

def read_orignal_file(name):
  # temporary function
  with open(name) as f:
    csv_file = csv.reader(f)
    all_data = []
    for row in csv_file:
      all_data.append(row)
    
    ref_tokens, ref_spks, rev_tokens, rev_spks = all_data[0], all_data[1], all_data[2], all_data[3]
    return ref_tokens, ref_spks, rev_tokens, rev_spks
    
def read_alignment(name):
  with open(name) as f:
    csv_file = csv.reader(f)
    all_data = []
    for row in csv_file:
      all_data.append(row)
    target = all_data[0]
    speaker_sequences = all_data[1:]
    hyp2ref_align = align_hyp_tokens_to_ref_speakers(target=target, speaker_sequences=speaker_sequences)
    aligned_sequences = get_sequence2target_alignment(target=target, speaker_sequences=speaker_sequences)
  
    return hyp2ref_align, aligned_sequences

def align_hyp_tokens_to_ref_speakers(target, speaker_sequences):
  hyp2ref_align = []
  for index, token in enumerate(target):
    gap = True
    if token != "-":
      for i in range(len(speaker_sequences)):
        if speaker_sequences[i][index] != "-":
          hyp2ref_align.append(i)
          gap = False
          break
      if gap:
        hyp2ref_align.append(-1)
  return hyp2ref_align

def get_sequence2target_alignment(target, speaker_sequences):
  aligned_sequences = []
  for sequence in speaker_sequences:
    tmp_res = []
    target_gap = 0
    for index, token in enumerate(sequence):
      if token != '-' and target[index] != '-':
        tmp_res.append(index-target_gap)
      elif token != '-' and target[index] == '-':
        target_gap += 1
        tmp_res.append(-1)
      elif token == '-' and target[index] == '-':
        target_gap += 1
    aligned_sequences.append(tmp_res)
  return aligned_sequences
        
        

if __name__ == "__main__":
  # read_orignal_file("data/Bdb001_all_tokens.csv")
  read_alignment("data/Bdb001_all_tokens_output.csv")