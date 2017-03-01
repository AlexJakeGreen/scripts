package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.Auditorium;
import ua.kh.bars.repositories.AuditoriumRepository;
import ua.kh.bars.services.AuditoriumService;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Service
@Transactional
public class AuditoriumServiceImpl implements AuditoriumService {

    @Autowired
    AuditoriumRepository auditoriumRepository;

    public List<Auditorium> findAll() {
        return auditoriumRepository.findAll();
    }
}
